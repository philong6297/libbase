// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "base/strings/string_utils.h"

namespace longlp::base {
#define LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII(CharType)       \
  auto ToLowerASCII(StringView##CharType str)->String##CharType { \
    return internal::ToLowerASCII<Char##CharType>(str);           \
  }                                                               \
  auto ToUpperASCII(StringView##CharType str)->String##CharType { \
    return internal::ToUpperASCII<Char##CharType>(str);           \
  }

LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII(ASCII)
LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII(UTF8)
LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII(UTF16)
LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII(UTF32)

#undef LONGLP_DEFINE_TO_LOWER_AND_TO_UPPER_ASCII

#define LONGLP_DEFINE_REMOVE_CHARS(CharType)                                \
  auto RemoveChars(                                                         \
    StringView##CharType input,                                             \
    StringView##CharType remove_chars,                                      \
    String##CharType& output)                                               \
    ->bool {                                                                \
    return internal::ReplaceChars<                                          \
      Char##CharType>(input, remove_chars, StringView##CharType(), output); \
  }
LONGLP_DEFINE_REMOVE_CHARS(ASCII)
LONGLP_DEFINE_REMOVE_CHARS(UTF8)
LONGLP_DEFINE_REMOVE_CHARS(UTF16)
LONGLP_DEFINE_REMOVE_CHARS(UTF32)

#undef LONGLP_DEFINE_REMOVE_CHARS

#define LONGLP_DEFINE_REPLACE_CHARS(CharType)                      \
  auto ReplaceChars(                                               \
    StringView##CharType input,                                    \
    StringView##CharType replace_chars,                            \
    StringView##CharType replace_with,                             \
    String##CharType& output)                                      \
    ->bool {                                                       \
    return internal::ReplaceChars<                                 \
      Char##CharType>(input, replace_chars, replace_with, output); \
  }
LONGLP_DEFINE_REPLACE_CHARS(ASCII)
LONGLP_DEFINE_REPLACE_CHARS(UTF8)
LONGLP_DEFINE_REPLACE_CHARS(UTF16)
LONGLP_DEFINE_REPLACE_CHARS(UTF32)

#undef LONGLP_DEFINE_REPLACE_CHARS

#define LONGLP_DEFINE_TRIM_STRING(CharType)          \
  auto TrimString(                                   \
    StringView##CharType input,                      \
    StringView##CharType trim_chars,                 \
    String##CharType& output)                        \
    ->bool {                                         \
    return internal::TrimString<Char##CharType>(     \
             input,                                  \
             trim_chars,                             \
             TrimPositions::kTrimAll,                \
             output) != TrimPositions::kTrimNone;    \
  }                                                  \
  auto TrimString(                                   \
    StringView##CharType input,                      \
    StringView##CharType trim_chars,                 \
    TrimPositions positions)                         \
    ->StringView##CharType {                         \
    return internal::TrimStringView<                 \
      Char##CharType>(input, trim_chars, positions); \
  }

LONGLP_DEFINE_TRIM_STRING(ASCII)
LONGLP_DEFINE_TRIM_STRING(UTF8)
LONGLP_DEFINE_TRIM_STRING(UTF16)
LONGLP_DEFINE_TRIM_STRING(UTF32)

#undef LONGLP_DEFINE_TRIM_STRING

#define LONGLP_DEFINE_TRIM_WHITESPACE(CharType)                            \
  auto TrimWhitespace(                                                     \
    StringView##CharType input,                                            \
    TrimPositions positions,                                               \
    String##CharType& output)                                              \
    ->TrimPositions {                                                      \
    return internal::TrimString<                                           \
      Char##CharType>(input, kWhitespace##CharType, positions, output);    \
  }                                                                        \
  auto TrimWhitespace(StringView##CharType input, TrimPositions positions) \
    ->StringView##CharType {                                               \
    return internal::TrimStringView<                                       \
      Char##CharType>(input, kWhitespace##CharType, positions);            \
  }
LONGLP_DEFINE_TRIM_WHITESPACE(UTF8)
LONGLP_DEFINE_TRIM_WHITESPACE(UTF16)
LONGLP_DEFINE_TRIM_WHITESPACE(UTF32)

#undef LONGLP_DEFINE_TRIM_WHITESPACE

#define LONGLP_DEFINE_TRIM_WHITESPACE_ASCII(CharType)                          \
  auto TrimWhitespaceASCII(                                                    \
    StringView##CharType input,                                                \
    TrimPositions positions,                                                   \
    String##CharType& output)                                                  \
    ->TrimPositions {                                                          \
    return internal::TrimString<                                               \
      Char##CharType>(input, kWhitespaceASCIIAs##CharType, positions, output); \
  }                                                                            \
  auto                                                                         \
  TrimWhitespaceASCII(StringView##CharType input, TrimPositions positions)     \
    ->StringView##CharType {                                                   \
    return internal::TrimStringView<                                           \
      Char##CharType>(input, kWhitespaceASCIIAs##CharType, positions);         \
  }
LONGLP_DEFINE_TRIM_WHITESPACE_ASCII(UTF8)
LONGLP_DEFINE_TRIM_WHITESPACE_ASCII(UTF16)
LONGLP_DEFINE_TRIM_WHITESPACE_ASCII(UTF32)

auto TrimWhitespaceASCII(
  StringViewASCII input,
  TrimPositions positions,
  StringASCII& output) -> TrimPositions {
  return internal ::TrimString<
    CharASCII>(input, kWhitespaceASCII, positions, output);
}

auto TrimWhitespaceASCII(StringViewASCII input, TrimPositions positions)
  -> StringViewASCII {
  return internal ::TrimStringView<
    CharASCII>(input, kWhitespaceASCII, positions);
}

#undef LONGLP_DEFINE_TRIM_WHITESPACE_ASCII
}    // namespace longlp::base
