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

#define LONGLP_DECLARE_REPLACE_CHARS(CharType)                     \
  auto ReplaceChars(                                               \
    StringView##CharType input,                                    \
    StringView##CharType replace_chars,                            \
    StringView##CharType replace_with,                             \
    String##CharType& output)                                      \
    ->bool {                                                       \
    return internal::ReplaceChars<                                 \
      Char##CharType>(input, replace_chars, replace_with, output); \
  }
LONGLP_DECLARE_REPLACE_CHARS(ASCII)
LONGLP_DECLARE_REPLACE_CHARS(UTF8)
LONGLP_DECLARE_REPLACE_CHARS(UTF16)
LONGLP_DECLARE_REPLACE_CHARS(UTF32)

#undef LONGLP_DECLARE_REPLACE_CHARS
}    // namespace longlp::base
