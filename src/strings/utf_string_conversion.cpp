// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "base/strings/utf_string_conversion.h"

#include <bit>
#include <climits>
#include <concepts>
#include <type_traits>

#include "base/icu/utf.h"
#include "base/strings/utf_string_conversion_utils.h"

namespace longlp::base {
// NOLINTBEGIN(*-magic-numbers)
namespace {

  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)

  constexpr icu::CodePoint kErrorCodePoint(0xFFFD);

  // Size coefficient ----------------------------------------------------------
  // The maximum number of codeunits in the destination encoding corresponding
  // to one codeunit in the source encoding.
  template <CharTraits SrcChar, CharTraits DestChar>
  consteval auto SizeCoefficient() -> int32_t {
    // clang-format off

    //  1 Character ~ 1 Codepoints
    //  UTF-8 code units: 1-byte
    //  UTF-16 code units: 2-byte
    //  UTF-32 code units: 4-byte
    //
    //  Bytes needed in:
    //  Codepoint   U+0000-U+007F     U+0080-U+07FF     U+0800-U+FFFF   U+10000-U+10FFFF
    //  UTF-8       1                 2                 3               4
    //  UTF-16      2                 2                 2               4
    //  UTF-32      4                 4                 4               4
    //
    //  Code units needed in
    //  Codepoint   U+0000-U+007F     U+0080-U+07FF     U+0800-U+FFFF   U+10000-U+10FFFF
    //  UTF-8       1                 2                 3               4
    //  UTF-16      1                 1                 1               2
    //  UTF-32      1                 1                 1               1

    // clang-format on

    // UTF-32 to UTF-16
    if constexpr (
      std::same_as<SrcChar, CharUTF32> && std::same_as<DestChar, CharUTF16>) {
      return 2;
    }

    // UTF-32 to UTF-8
    if constexpr (
      std::same_as<SrcChar, CharUTF32> && std::same_as<DestChar, CharUTF8>) {
      return 4;
    }

    // UTF-16 to UTF-8
    if constexpr (
      std::same_as<SrcChar, CharUTF32> && std::same_as<DestChar, CharUTF8>) {
      return 3;
    }

    // UTF-32 to ASCII
    // UTF-16 to ASCII
    // UTF-8 to ASCII
    // ASCII symbols are encoded by one codeunit in all encodings.

    return 1;
  }

  // UnicodeAppendUnsafe
  // -------------------------------------------------------- Function overloads
  // that write code_point to the output string. Output string has to have
  // enough space for the codepoint.

  // Convenience typedef that checks whether the passed in type is integral
  // (i.e. bool, char, int or their extended versions) and is of the correct
  // size.
  template <CharTraits Char>
  consteval auto SizeInBits() -> size_t {
    return CHAR_BIT * sizeof(Char);
  }

  template <CharTraits Char>
  requires(SizeInBits<Char>() == 8U)
  void UnicodeAppendUnsafe(
    Char* out,
    size_t& size,
    base::icu::CodePoint code_point) {
    icu::internal::
      U8AppendUnsafe(std::bit_cast<uint8_t*>(out), size, code_point);
  }

  template <CharTraits Char>
  requires(SizeInBits<Char>() == 16U)
  void UnicodeAppendUnsafe(
    Char* out,
    size_t& size,
    base::icu::CodePoint code_point) {
    icu::internal::U16AppendUnsafe(out, size, code_point);
  }

  template <CharTraits Char>
  requires(SizeInBits<Char>() == 32U)
  void UnicodeAppendUnsafe(
    Char* out,
    size_t& size,
    base::icu::CodePoint code_point) {
    out[size] = static_cast<Char>(code_point);
    ++size;
  }

  // DoUTFConversion
  // ------------------------------------------------------------ Main driver of
  // UTFConversion specialized for different Src encodings. dest has to have
  // enough room for the converted text.

  template <CharTraits DestChar>
  auto DoUTFConversion(
    const StringViewUTF8 src,
    std::basic_string<DestChar>& dest,
    const size_t dest_offset) -> bool {
    bool success = true;
    dest.reserve(dest_offset + src.size());

    auto start_offset = dest_offset;

    for (size_t i = 0; i < src.size();) {
      base::icu::CodePoint code_point;
      icu::internal::U8Next(
        std::bit_cast<const uint8_t*>(src.data()),
        i,
        static_cast<int32_t>(src.size()),
        *code_point);

      if (!IsValidCodepoint(code_point)) {
        success    = false;
        code_point = kErrorCodePoint;
      }

      UnicodeAppendUnsafe(dest, start_offset, code_point);
    }

    return success;
  }

  template <CharTraits DestChar>
  auto DoUTFConversion(
    const StringViewUTF16 src,
    std::basic_string<DestChar>& dest,
    const size_t dest_offset) -> bool {
    bool success = true;
    dest.reserve(dest_offset + src.size());

    auto convert_single_char = [&success](char16_t input) -> icu::CodePoint {
      icu::CodePoint code_point(input);
      if (!icu::internal::U16IsSingle(input) || !IsValidCodepoint(code_point)) {
        success    = false;
        code_point = kErrorCodePoint;
      }
      return code_point;
    };

    size_t i          = 0U;
    auto start_offset = dest_offset;

    // Always have another symbol in order to avoid checking boundaries in the
    // middle of the surrogate pair.
    while (i + 1 < src.size()) {
      base::icu::CodePoint code_point;

      if (icu::internal::U16IsLead(src[i]) &&
          icu::internal::U16IsTrail(src[i + 1])) {
        *code_point = icu::internal::U16GetSupplementary(src[i], src[i + 1]);
        if (!IsValidCodepoint(code_point)) {
          code_point = kErrorCodePoint;
          success    = false;
        }
        i += 2;
      }
      else {
        code_point = convert_single_char(src[i]);
        ++i;
      }

      UnicodeAppendUnsafe(dest, start_offset, code_point);
    }

    if (i < src.size()) {
      UnicodeAppendUnsafe(dest, start_offset, convert_single_char(src[i]));
    }

    return success;
  }

  template <CharTraits DestChar>
  auto DoUTFConversion(
    const StringViewUTF32 src,
    std::basic_string<DestChar>& dest,
    const size_t dest_offset) -> bool {
    bool success = true;
    dest.reserve(dest_offset + src.size());

    auto start_offset = dest_offset;
    for (CharUTF32 character : src) {
      icu::CodePoint code_point(static_cast<UChar32>(character));

      if (!IsValidCodepoint(code_point)) {
        success    = false;
        code_point = kErrorCodePoint;
      }

      UnicodeAppendUnsafe(dest, start_offset, code_point);
    }

    return success;
  }

  // UTFConversion
  // -------------------------------------------------------------- Function
  // template for generating all UTF conversions.

  template <typename InputString, typename DestString>
  bool UTFConversion(const InputString& src_str, DestString* dest_str) {
    if (IsStringASCII(src_str)) {
      dest_str->assign(src_str.begin(), src_str.end());
      return true;
    }

    dest_str->resize(
      src_str.length() *
      size_coefficient_v<
        typename InputString::value_type,
        typename DestString::value_type>);

    // Empty string is ASCII => it OK to call operator[].
    auto* dest = &(*dest_str)[0];

    // ICU requires 32 bit numbers.
    size_t src_len  = src_str.length();
    size_t dest_len = 0;

    bool res        = DoUTFConversion(src_str.data(), src_len, dest, &dest_len);

    dest_str->resize(dest_len);
    dest_str->shrink_to_fit();

    return res;
  }

  // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}    // namespace

// ASCII To Others
auto ASCIIToUTF16(StringViewASCII ascii, StringUTF16& utf16_output) -> bool {
  utf16_output.reserve(ascii.size());
  utf16_output = {ascii.begin(), ascii.end()};
  return true;
}

auto ASCIIToUTF32(StringViewASCII ascii, StringUTF32& utf32_output) -> bool {
  utf32_output.reserve(ascii.size());
  utf32_output = {ascii.begin(), ascii.end()};
  return true;
}

auto ASCIIToUTF8(StringViewASCII ascii, StringUTF8& utf8_output) -> bool {
  utf8_output.reserve(ascii.size());
  utf8_output = {ascii.begin(), ascii.end()};
  return true;
}

// UTF32 To Others
auto UTF32ToUTF8(StringViewUTF32 utf32, StringUTF8& utf8_output) -> bool {}

auto UTF32ToUTF16(StringViewUTF32 utf32, StringUTF16& utf16_output) -> bool {}

auto UTF32ToASCII(StringViewUTF32 utf32, StringASCII& ascii_output) -> bool {}

// NOLINTEND(*-magic-numbers)
}    // namespace longlp::base
