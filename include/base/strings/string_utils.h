// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

// This file defines utility functions for working with strings.
// Unfortunately, User-defined Deduction guide don't work on function templates.
// It only works with class templates. Thus, some fuctions will have to declare
// explicity for each variants

#ifndef LONGLP_INCLUDE_BASE_STRINGS_STRING_UTIL_H_
#define LONGLP_INCLUDE_BASE_STRINGS_STRING_UTIL_H_

#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <vector>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/strings/string_utils.internal.h"
#include "base/strings/typedefs.h"

namespace longlp::base {
// ASCII-specific tolower/toupper. The standard library's tolower/toupper is
// locale sensitive, so we don't want to use it here.
#define LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_CHAR(CharType) \
  BASE_EXPORT constexpr auto ToLowerASCII(Char##CharType char_value) \
    ->Char##CharType {                                               \
    return internal::ToLowerASCII<Char##CharType>(char_value);       \
  }                                                                  \
  BASE_EXPORT constexpr auto ToUpperASCII(Char##CharType char_value) \
    ->Char##CharType {                                               \
    return internal::ToUpperASCII<Char##CharType>(char_value);       \
  }

LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_CHAR(ASCII)
LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_CHAR(UTF8)
LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_CHAR(UTF16)
LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_CHAR(UTF32)

#undef LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_CHAR

// Converts the given string to its ASCII-lowercase/uppercase equivalent.
// Non-ASCII bytes (or UTF-16 code units in `StringViewUTF16`) are permitted but
// will be unmodified.
#define LONGLP_DECLARE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_STRING(CharType)      \
  BASE_EXPORT auto ToLowerASCII(StringView##CharType str)->String##CharType; \
  BASE_EXPORT auto ToUpperASCII(StringView##CharType str)->String##CharType;

LONGLP_DECLARE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_STRING(ASCII)
LONGLP_DECLARE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_STRING(UTF8)
LONGLP_DECLARE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_STRING(UTF16)
LONGLP_DECLARE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_STRING(UTF32)

#undef LONGLP_DECLARE_TO_LOWER_AND_TO_UPPER_ASCII_FOR_STRING

// Functor for ASCII case-insensitive comparisons for STL algorithms like
// std::search. Non-ASCII bytes (or UTF-16 code units in `StringViewUTF16`) are
// permitted but will be compared as-is.
//
// Note that a full Unicode version of this functor is not possible to write
// because case mappings might change the number of characters, depend on
// context (combining accents), and require handling UTF-16. If you need
// proper Unicode support, use base::i18n::ToLower/FoldCase and then just
// use a normal operator== on the result.
template <CharTraits Char>
struct CaseInsensitiveCompareASCII {
 public:
  constexpr auto operator()(Char lhs, Char rhs) const -> bool {
    return ToLowerASCII(lhs) == ToLowerASCII(rhs);
  }
};

// Like strcasecmp for ASCII case-insensitive comparisons only. Returns:
//   -1  (a < b)
//    0  (a == b)
//    1  (a > b)
// To compare all Unicode code points case-insensitively, use
// base::i18n::ToLower or base::i18n::FoldCase and then just call the normal
// string operators on the result.
//
// Non-ASCII bytes (or UTF-16 code units in `StringViewUTF16`) are permitted but
// will be compared unmodified.

#define LONGLP_DEFINE_COMPARE_CASE_INSENSITIVE_ASCII(CharType)              \
  BASE_EXPORT constexpr auto CompareCaseInsensitiveASCII(                   \
    StringView##CharType lhs,                                               \
    StringView##CharType rhs)                                               \
    ->int32_t {                                                             \
    return internal::CompareCaseInsensitiveASCII<Char##CharType>(lhs, rhs); \
  }

LONGLP_DEFINE_COMPARE_CASE_INSENSITIVE_ASCII(ASCII)
LONGLP_DEFINE_COMPARE_CASE_INSENSITIVE_ASCII(UTF8)
LONGLP_DEFINE_COMPARE_CASE_INSENSITIVE_ASCII(UTF16)
LONGLP_DEFINE_COMPARE_CASE_INSENSITIVE_ASCII(UTF32)

#undef LONGLP_DEFINE_COMPARE_CASE_INSENSITIVE_ASCII

// Equality for ASCII case-insensitive comparisons. Non-ASCII bytes (or UTF-16
// code units in `StringViewUTF16`) are permitted but will be compared
// unmodified. To compare all Unicode code points case-insensitively, use
// base::i18n::ToLower or base::i18n::FoldCase and then compare with either ==
// or !=.

#define LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(CharA, CharB)      \
  BASE_EXPORT constexpr auto                                               \
  EqualsCaseInsensitiveASCII(StringView##CharA lhs, StringView##CharB rhs) \
    ->bool {                                                               \
    return internal::                                                      \
      EqualsCaseInsensitiveASCII<Char##CharA, Char##CharB>(lhs, rhs);      \
  }

LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF8, UTF8)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF8, UTF16)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF8, UTF32)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF8, ASCII)

LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF16, UTF8)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF16, UTF16)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF16, UTF32)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF16, ASCII)

LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF32, UTF8)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF32, UTF16)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF32, UTF32)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(UTF32, ASCII)

LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(ASCII, UTF8)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(ASCII, UTF16)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(ASCII, UTF32)
LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR(ASCII, ASCII)

#undef LONGLP_DEFINE_EQUALS_CASE_INSENSITIVE_ASCII_FOR

// Removes characters in |remove_chars| from anywhere in |input|.  Returns true
// if any characters were removed.  |remove_chars| must be null-terminated.
// NOTE: Safe to use the same variable for both |input| and |output|.
#define LONGLP_DECLARE_REMOVE_CHARS(CharType) \
  BASE_EXPORT auto RemoveChars(               \
    StringView##CharType input,               \
    StringView##CharType remove_chars,        \
    String##CharType& output)                 \
    ->bool;
LONGLP_DECLARE_REMOVE_CHARS(ASCII)
LONGLP_DECLARE_REMOVE_CHARS(UTF8)
LONGLP_DECLARE_REMOVE_CHARS(UTF16)
LONGLP_DECLARE_REMOVE_CHARS(UTF32)

#undef LONGLP_DECLARE_REMOVE_CHARS

// Replaces characters in |replace_chars| from anywhere in |input| with
// |replace_with|.  Each character in |replace_chars| will be replaced with
// the |replace_with| string.  Returns true if any characters were replaced.
// |replace_chars| must be null-terminated.
// NOTE: Safe to use the same variable for both |input| and |output|.
#define LONGLP_DECLARE_REPLACE_CHARS(CharType) \
  BASE_EXPORT auto ReplaceChars(               \
    StringView##CharType input,                \
    StringView##CharType replace_chars,        \
    StringView##CharType replace_with,         \
    String##CharType& output)                  \
    ->bool;
LONGLP_DECLARE_REPLACE_CHARS(ASCII)
LONGLP_DECLARE_REPLACE_CHARS(UTF8)
LONGLP_DECLARE_REPLACE_CHARS(UTF16)
LONGLP_DECLARE_REPLACE_CHARS(UTF32)

#undef LONGLP_DECLARE_REPLACE_CHARS

using internal::TrimPositions;
// Removes characters in |trim_chars| from the beginning and end of |input|.
// The 8-bit version only works on 8-bit characters, not UTF-8. Returns true if
// any characters were removed.
//
// It is safe to use the same variable for both |input| and |output| (this is
// the normal usage to trim in-place).
// StringView versions of the above. The returned pieces refer to the original
// buffer.
#define LONGLP_DECLARE_TRIM_STRING(CharType) \
  BASE_EXPORT auto TrimString(               \
    StringView##CharType input,              \
    StringView##CharType trim_chars,         \
    String##CharType& output)                \
    ->bool;                                  \
  BASE_EXPORT auto TrimString(               \
    StringView##CharType input,              \
    StringView##CharType trim_chars,         \
    TrimPositions positions)                 \
    ->StringView##CharType;

LONGLP_DECLARE_TRIM_STRING(ASCII)
LONGLP_DECLARE_TRIM_STRING(UTF8)
LONGLP_DECLARE_TRIM_STRING(UTF16)
LONGLP_DECLARE_TRIM_STRING(UTF32)

#undef LONGLP_DECLARE_TRIM_STRING

// Truncates a string to the nearest UTF-8 character that will leave
// the string less than or equal to the specified byte size.
BASE_EXPORT void TruncateUTF8ToByteSize(
  StringViewUTF8 input,
  size_t byte_size,
  StringUTF8& output);

// Trims any whitespace from either end of the input string.
//
// The StringView versions return a substring referencing the input buffer.
// The ASCII versions look only for ASCII whitespace.
//
// The String versions return where whitespace was found.
// NOTE: Safe to use the same variable for both input and output.
#define LONGLP_DECLARE_TRIM_WHITESPACE(CharType)                      \
  BASE_EXPORT auto TrimWhitespace(                                    \
    StringView##CharType input,                                       \
    TrimPositions positions,                                          \
    String##CharType& output)                                         \
    ->TrimPositions;                                                  \
  BASE_EXPORT auto                                                    \
  TrimWhitespace(StringView##CharType input, TrimPositions positions) \
    ->StringView##CharType;
LONGLP_DECLARE_TRIM_WHITESPACE(UTF8)
LONGLP_DECLARE_TRIM_WHITESPACE(UTF16)
LONGLP_DECLARE_TRIM_WHITESPACE(UTF32)

#undef LONGLP_DECLARE_TRIM_WHITESPACE

#define LONGLP_DECLARE_TRIM_WHITESPACE_ASCII(CharType)                     \
  BASE_EXPORT auto TrimWhitespaceASCII(                                    \
    StringView##CharType input,                                            \
    TrimPositions positions,                                               \
    String##CharType& output)                                              \
    ->TrimPositions;                                                       \
  BASE_EXPORT auto                                                         \
  TrimWhitespaceASCII(StringView##CharType input, TrimPositions positions) \
    ->StringView##CharType;
LONGLP_DECLARE_TRIM_WHITESPACE_ASCII(ASCII)
LONGLP_DECLARE_TRIM_WHITESPACE_ASCII(UTF8)
LONGLP_DECLARE_TRIM_WHITESPACE_ASCII(UTF16)
LONGLP_DECLARE_TRIM_WHITESPACE_ASCII(UTF32)

#undef LONGLP_DECLARE_TRIM_WHITESPACE_ASCII

// Determines the type of ASCII character, independent of locale (the C
// library versions will change based on locale).

#define LONGLP_DEFINE_IS_ASCII_WHITESPACE(CharType)                        \
  BASE_EXPORT constexpr auto IsASCIIWhitespace(Char##CharType val)->bool { \
    return internal::                                                      \
      IsASCIIWhitespace<Char##CharType, class CharTag##CharType>(val);     \
  }
LONGLP_DEFINE_IS_ASCII_WHITESPACE(UTF8)
LONGLP_DEFINE_IS_ASCII_WHITESPACE(UTF16)
LONGLP_DEFINE_IS_ASCII_WHITESPACE(UTF32)
LONGLP_DEFINE_IS_ASCII_WHITESPACE(ASCII)

#undef LONGLP_DEFINE_IS_ASCII_WHITESPACE
}    // namespace longlp::base
#endif    // LONGLP_INCLUDE_BASE_STRINGS_STRING_UTIL_H_
