// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_STRINGS_UTF_STRING_CONVERSION_UTILS_H_
#define LONGLP_INCLUDE_BASE_STRINGS_UTF_STRING_CONVERSION_UTILS_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/icu/utf.h"
#include "base/strings/typedefs.h"

namespace longlp::base {

// NOLINTBEGIN(*-magic-numbers)

LONGLP_ALWAYS_INLINE constexpr auto
IsValidCodepoint(icu::CodePoint code_point) -> bool {
  // Excludes code points that are not Unicode scalar values, i.e.
  // surrogate code points ([0xD800, 0xDFFF]). Additionally, excludes
  // code points larger than 0x10FFFF (the highest codepoint allowed).
  // Non-characters and unassigned code points are allowed.
  // https://unicode.org/glossary/#unicode_scalar_value
  return (code_point.value() >= 0 && code_point.value() < 0xD800) ||
         (code_point.value() >= 0xE000 && code_point.value() <= 0x10FFFF);
}

LONGLP_ALWAYS_INLINE constexpr auto
IsValidCharacter(icu::CodePoint code_point) -> bool {
  // Excludes non-characters (U+FDD0..U+FDEF, and all code points
  // ending in 0xFFFE or 0xFFFF) from the set of valid code points.
  // https://unicode.org/faq/private_use.html#nonchar1
  // NOLINTBEGIN(hicpp-signed-bitwise)
  return (code_point.value() >= 0 && code_point.value() < 0xD800) ||
         (code_point.value() >= 0xE000 && code_point.value() < 0xFDD0) ||
         (code_point.value() > 0xFDEF && code_point.value() <= 0x10FFFF &&
          (code_point.value() & 0xFFFE) != 0xFFFE);
  // NOLINTEND(hicpp-signed-bitwise)
}

// ReadUnicodeCharacter --------------------------------------------------------

// Reads a UTF-8 stream, placing the next code point into the given output
// |code_point|. |src| represents the entire string to read, and |char_index|
// is the character offset within the string to start reading at. |char_index|
// will be updated to index the last character read, such that incrementing it
// (as in a for loop) will take the reader to the next character.
// TODO(longlp, c++23): Use std::expected
//
// Returns true on success. On false, |code_point| will be invalid.
BASE_EXPORT auto ReadUnicodeCharacter(
  StringViewUTF8 utf8_src,
  size_t& char_index,
  icu::CodePoint& code_point_out) -> bool;

// Reads a UTF-16 character. The usage is the same as the 8-bit version above.
BASE_EXPORT auto ReadUnicodeCharacter(
  StringViewUTF16 utf16_src,
  size_t& char_index,
  icu::CodePoint& code_point_out) -> bool;

// Reads UTF-32 character. The usage is the same as the 8-bit version above.
BASE_EXPORT auto ReadUnicodeCharacter(
  StringViewUTF32 utf32_src,
  size_t& char_index,
  icu::CodePoint& code_point_out) -> bool;

// AppendUnicodeCharacter
// -------------------------------------------------------

// Appends a UTF-8 character to the given 8-bit string.  Returns the number of
// bytes written.
BASE_EXPORT auto
AppendUnicodeCharacter(icu::CodePoint code_point, StringUTF8& utf8_output)
  -> size_t;

// Appends the given code point as a UTF-16 character to the given 16-bit
// string.  Returns the number of 16-bit values written.
BASE_EXPORT auto
AppendUnicodeCharacter(icu::CodePoint code_point, StringUTF16& utf16_output)
  -> size_t;

// Appends the given UTF-32 character to the given 32-bit string.  Returns the
// number of 32-bit values written.
LONGLP_ALWAYS_INLINE auto
AppendUnicodeCharacter(icu::CodePoint code_point, StringUTF32& utf32_output)
  -> size_t {
  // This is the easy case, just append the character.
  utf32_output.push_back(static_cast<CharUTF32>(code_point.value()));
  return 1;
}

// Generalized Unicode converter -----------------------------------------------

// Guesses the length of the output in UTF-8 in bytes, clears that output
// string, and reserves that amount of space.  We assume that the input
// character types are unsigned, which will be true for UTF-16 and -32 on our
// systems.
template <CharTraits CharT>
void PrepareForUTF8Output(
  std::basic_string_view<CharT> src,
  StringUTF8& utf8_output);

// Prepares an output buffer (containing either UTF-16 or -32 data) given some
// UTF-8 input that will be converted to it.  See PrepareForUTF8Output().
template <CharTraits CharT>
void PrepareForUTF16Or32Output(
  StringViewUTF8 utf8_src,
  std::basic_string<CharT>& output);

// NOLINTEND(*-magic-numbers)
}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_STRINGS_UTF_STRING_CONVERSION_UTILS_H_
