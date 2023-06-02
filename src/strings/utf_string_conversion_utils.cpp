// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "base/strings/utf_string_conversion_utils.h"
#include <bit>

namespace longlp::base {

// NOLINTBEGIN(*-magic-numbers)

// ReadUnicodeCharacter --------------------------------------------------------

auto ReadUnicodeCharacter(
  const std::string_view utf8_src,
  size_t& char_index,
  icu::U32CodePoint& code_point_out) -> bool {
  icu::internal::U8Next(
    std::bit_cast<const uint8_t*>(utf8_src.data()),
    char_index,
    static_cast<int32_t>(utf8_src.length()),
    *code_point_out);

  // The ICU macro above moves to the next char, we want to point to the last
  // char consumed.
  --char_index;

  // Validate the decoded value.
  return IsValidCodepoint(code_point_out);
}

auto ReadUnicodeCharacter(
  const std::u16string_view utf16_src,
  size_t& char_index,
  icu::U32CodePoint& code_point_out) -> bool {
  if (icu::internal::U16IsSurrogate(utf16_src[char_index])) {
    if (
      !icu::internal::U16IsSurrogateLead(utf16_src[char_index]) ||
      utf16_src.empty() || char_index >= utf16_src.length() - 1 ||
      !icu::internal::U16IsTrail(utf16_src[char_index + 1])) {
      // Invalid surrogate pair.
      return false;
    }

    // Valid surrogate pair.
    *code_point_out = icu::internal::
      U16GetSupplementary(utf16_src[char_index], utf16_src[char_index + 1]);
    ++char_index;
  }
  else {
    // Not a surrogate, just one 16-bit word.
    *code_point_out = utf16_src[char_index];
  }

  return IsValidCodepoint(code_point_out);
}

auto ReadUnicodeCharacter(
  const std::u32string_view utf32_src,
  size_t& char_index,
  icu::U32CodePoint& code_point_out) -> bool {
  // Conversion is easy since the source is 32-bit.
  *code_point_out =
    static_cast<icu::U32CodePoint::UnderlyingType>(utf32_src[char_index]);

  // Validate the value.
  return IsValidCodepoint(code_point_out);
}

// WriteUnicodeCharacter -------------------------------------------------------

auto AppendUnicodeCharacter(
  const icu::U32CodePoint code_point,
  std::string& utf8_output) -> size_t {
  if (code_point.value() >= 0 && code_point.value() <= 0x7f) {
    // Fast path the common case of one byte.
    utf8_output.push_back(static_cast<char>(code_point.value()));
    return 1;
  }

  // U8_APPEND_UNSAFE can append up to 4 bytes.
  size_t char_offset          = utf8_output.length();
  size_t original_char_offset = char_offset;
  utf8_output.resize(char_offset + icu::internal::kU8MaxLength);

  icu::internal::U8AppendUnsafe(
    std::bit_cast<uint8_t*>(utf8_output.data()),
    char_offset,
    *code_point);

  // U8_APPEND_UNSAFE will advance our pointer past the inserted character, so
  // it will represent the new length of the string.
  utf8_output.resize(char_offset);
  return char_offset - original_char_offset;
}

auto AppendUnicodeCharacter(
  const icu::U32CodePoint code_point,
  std::u16string& utf16_output) -> size_t {
  if (icu::internal::U16Length(code_point.value()) == 1) {
    // The code point is in the Basic Multilingual Plane (BMP).
    utf16_output.push_back(static_cast<char16_t>(code_point.value()));
    return 1;
  }
  // Non-BMP characters use a double-character encoding.
  size_t char_offset = utf16_output.length();
  utf16_output.resize(char_offset + icu::internal::kU16MaxLength);
  icu::internal::
    U16AppendUnsafe(&(utf16_output.front()), char_offset, *code_point);
  return icu::internal::kU16MaxLength;
}

// Generalized Unicode converter -----------------------------------------------

template <typename CharT>
void PrepareForUTF8Output(
  const std::basic_string_view<CharT> src,
  std::string& utf8_output) {
  utf8_output.clear();
  if (src.empty()) {
    return;
  }
  if (src[0] < 0x80) {
    // Assume that the entire input will be ASCII.
    utf8_output.reserve(src.length());
    return;
  }

  // Assume that the entire input is non-ASCII and will have 3 bytes per char.
  utf8_output.reserve(src.length() * 3);
}

// Instantiate versions we know callers will need.
template void PrepareForUTF8Output(const std::u32string_view, std::string&);
template void PrepareForUTF8Output(const std::u16string_view, std::string&);

template <typename CharT>
void PrepareForUTF16Or32Output(
  std::string_view utf8_src,
  std::basic_string<CharT>& output) {
  output.clear();
  if (utf8_src.empty()) {
    return;
  }
  if (static_cast<char8_t>(utf8_src[0]) < 0x80) {
    // Assume the input is all ASCII, which means 1:1 correspondence.
    output.reserve(utf8_src.length());
    return;
  }

  // Otherwise assume that the UTF-8 sequences will have 2 bytes for each
  // character.
  output.reserve(utf8_src.length() / 2);
}

// Instantiate versions we know callers will need.
template void
PrepareForUTF16Or32Output(const std::string_view, std::u32string&);
template void
PrepareForUTF16Or32Output(const std::string_view, std::u16string&);

// NOLINTEND(*-magic-numbers)

}    // namespace longlp::base
