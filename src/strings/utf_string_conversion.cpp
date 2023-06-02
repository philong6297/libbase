// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "base/strings/utf_string_conversion.h"

#include <concepts>
#include <type_traits>

#include "base/icu/utf.h"

namespace longlp::base {
namespace {

  constexpr icu::U32CodePoint kErrorCodePoint(0xFFFD);

  // Size coefficient ----------------------------------------------------------
  // The maximum number of codeunits in the destination encoding corresponding
  // to one codeunit in the source encoding.
  template <typename SrcChar, typename DestChar>
  consteval auto SizeCoefficient() -> int32_t {
    using SrcCharType  = std::decay_t<SrcChar>;
    using DestCharType = std::decay_t<DestChar>;

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
      std::same_as<SrcCharType, UTF32Char> &&
      std::same_as<DestCharType, UTF16Char>) {
      return 2;
    }

    // UTF-32 to UTF-8
    if constexpr (
      std::same_as<SrcCharType, UTF32Char> &&
      std::same_as<DestCharType, UTF8Char>) {
      return 4;
    }

    // UTF-16 to UTF-8
    if constexpr (
      std::same_as<SrcCharType, UTF32Char> &&
      std::same_as<DestCharType, UTF8Char>) {
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
  template <typename Char, size_t N>
  using EnableIfBitsAre = std::enable_if_t<
    std::is_integral<Char>::value && CHAR_BIT * sizeof(Char) == N,
    bool>;

  template <typename Char, EnableIfBitsAre<Char, 8> = true>
  void
  UnicodeAppendUnsafe(Char* out, size_t* size, base_icu::UChar32 code_point) {
    CBU8_APPEND_UNSAFE(reinterpret_cast<uint8_t*>(out), *size, code_point);
  }

  template <typename Char, EnableIfBitsAre<Char, 16> = true>
  void
  UnicodeAppendUnsafe(Char* out, size_t* size, base_icu::UChar32 code_point) {
    CBU16_APPEND_UNSAFE(out, *size, code_point);
  }

  template <typename Char, EnableIfBitsAre<Char, 32> = true>
  void
  UnicodeAppendUnsafe(Char* out, size_t* size, base_icu::UChar32 code_point) {
    out[(*size)++] = static_cast<Char>(code_point);
  }

  // DoUTFConversion
  // ------------------------------------------------------------ Main driver of
  // UTFConversion specialized for different Src encodings. dest has to have
  // enough room for the converted text.

  template <typename DestChar>
  bool DoUTFConversion(
    const char* src,
    size_t src_len,
    DestChar* dest,
    size_t* dest_len) {
    bool success = true;

    for (size_t i = 0; i < src_len;) {
      base_icu::UChar32 code_point;
      CBU8_NEXT(reinterpret_cast<const uint8_t*>(src), i, src_len, code_point);

      if (!IsValidCodepoint(code_point)) {
        success    = false;
        code_point = kErrorCodePoint;
      }

      UnicodeAppendUnsafe(dest, dest_len, code_point);
    }

    return success;
  }

  template <typename DestChar>
  bool DoUTFConversion(
    const char16_t* src,
    size_t src_len,
    DestChar* dest,
    size_t* dest_len) {
    bool success           = true;

    auto ConvertSingleChar = [&success](char16_t in) -> base_icu::UChar32 {
      if (!CBU16_IS_SINGLE(in) || !IsValidCodepoint(in)) {
        success = false;
        return kErrorCodePoint;
      }
      return in;
    };

    size_t i = 0;

    // Always have another symbol in order to avoid checking boundaries in the
    // middle of the surrogate pair.
    while (i + 1 < src_len) {
      base_icu::UChar32 code_point;

      if (CBU16_IS_LEAD(src[i]) && CBU16_IS_TRAIL(src[i + 1])) {
        code_point = CBU16_GET_SUPPLEMENTARY(src[i], src[i + 1]);
        if (!IsValidCodepoint(code_point)) {
          code_point = kErrorCodePoint;
          success    = false;
        }
        i += 2;
      }
      else {
        code_point = ConvertSingleChar(src[i]);
        ++i;
      }

      UnicodeAppendUnsafe(dest, dest_len, code_point);
    }

    if (i < src_len) {
      UnicodeAppendUnsafe(dest, dest_len, ConvertSingleChar(src[i]));
    }

    return success;
  }

#if defined(WCHAR_T_IS_UTF32)

  template <typename DestChar>
  bool DoUTFConversion(
    const wchar_t* src,
    size_t src_len,
    DestChar* dest,
    size_t* dest_len) {
    bool success = true;

    for (size_t i = 0; i < src_len; ++i) {
      auto code_point = static_cast<base_icu::UChar32>(src[i]);

      if (!IsValidCodepoint(code_point)) {
        success    = false;
        code_point = kErrorCodePoint;
      }

      UnicodeAppendUnsafe(dest, dest_len, code_point);
    }

    return success;
  }

#endif    // defined(WCHAR_T_IS_UTF32)

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
}    // namespace

// ASCII To Others
auto ASCIIToUTF16(ASCIIStringView ascii, UTF16String& utf16_output) -> bool {
  utf16_output.reserve(ascii.size());
  utf16_output = {ascii.begin(), ascii.end()};
  return true;
}

auto ASCIIToUTF32(ASCIIStringView ascii, UTF32String& utf32_output) -> bool {
  utf32_output.reserve(ascii.size());
  utf32_output = {ascii.begin(), ascii.end()};
  return true;
}

auto ASCIIToUTF8(ASCIIStringView ascii, UTF8String& utf8_output) -> bool {
  utf8_output.reserve(ascii.size());
  utf8_output = {ascii.begin(), ascii.end()};
  return true;
}

// UTF32 To Others
auto UTF32ToUTF8(UTF32StringView utf32, UTF8String& utf8_output) -> bool {}

auto UTF32ToUTF16(UTF32StringView utf32, UTF16String& utf16_output) -> bool {}

auto UTF32ToASCII(UTF32StringView utf32, ASCIIString& ascii_output) -> bool {}
}    // namespace longlp::base
