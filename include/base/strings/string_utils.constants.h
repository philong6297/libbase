// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_STRINGS_STRING_UTILS_CONSTANTS_H_
#define LONGLP_INCLUDE_BASE_STRINGS_STRING_UTILS_CONSTANTS_H_

#include "base/base_export.h"
#include "base/strings/typedefs.h"

namespace longlp::base {
// Contains the set of characters representing whitespace in the corresponding
// encoding. Null-terminated. The ASCII versions are the whitespaces as defined
// by HTML5, and don't include control characters.

#define LONGLP_WHITESPACE_ASCII_NO_CR_LF(CharType)                 \
  LONGLP_LITERAL_##CharType("\u0009")   /* CHARACTER TABULATION */ \
    LONGLP_LITERAL_##CharType("\u000B") /* LINE TABULATION */      \
    LONGLP_LITERAL_##CharType("\u000C") /* FORM FEED (FF) */       \
    LONGLP_LITERAL_##CharType("\u0020") /* SPACE */

#define LONGLP_WHITESPACE_ASCII(CharType)                         \
  LONGLP_WHITESPACE_ASCII_NO_CR_LF(CharType) /* Break comments */ \
  LONGLP_LITERAL_##CharType("\u000A")        /* LINE FEED (LF) */ \
    LONGLP_LITERAL_##CharType("\u000D")      /* CARRIAGE RETURN (CR) */

#define LONGLP_WHITESPACE_UNICODE_NON_ASCII(CharType)                   \
  LONGLP_LITERAL_##CharType("\u0085")   /* NEXT LINE (NEL) */           \
    LONGLP_LITERAL_##CharType("\u00A0") /* NO-BREAK SPACE */            \
    LONGLP_LITERAL_##CharType("\u1680") /* OGHAM SPACE MARK */          \
    LONGLP_LITERAL_##CharType("\u2000") /* EN QUAD */                   \
    LONGLP_LITERAL_##CharType("\u2001") /* EM QUAD */                   \
    LONGLP_LITERAL_##CharType("\u2002") /* EN SPACE */                  \
    LONGLP_LITERAL_##CharType("\u2003") /* EM SPACE */                  \
    LONGLP_LITERAL_##CharType("\u2004") /* THREE-PER-EM SPACE */        \
    LONGLP_LITERAL_##CharType("\u2005") /* FOUR-PER-EM SPACE */         \
    LONGLP_LITERAL_##CharType("\u2006") /* SIX-PER-EM SPACE */          \
    LONGLP_LITERAL_##CharType("\u2007") /* FIGURE SPACE */              \
    LONGLP_LITERAL_##CharType("\u2008") /* PUNCTUATION SPACE */         \
    LONGLP_LITERAL_##CharType("\u2009") /* THIN SPACE */                \
    LONGLP_LITERAL_##CharType("\u200A") /* HAIR SPACE */                \
    LONGLP_LITERAL_##CharType("\u2028") /* LINE SEPARATOR */            \
    LONGLP_LITERAL_##CharType("\u2029") /* PARAGRAPH SEPARATOR */       \
    LONGLP_LITERAL_##CharType("\u202F") /* NARROW NO-BREAK SPACE */     \
    LONGLP_LITERAL_##CharType("\u205F") /* MEDIUM MATHEMATICAL SPACE */ \
    LONGLP_LITERAL_##CharType("\u3000") /* IDEOGRAPHIC SPACE */

#define LONGLP_WHITESPACE_UNICODE_NO_CR_LF(CharType) \
  LONGLP_WHITESPACE_ASCII_NO_CR_LF(CharType)         \
  LONGLP_WHITESPACE_UNICODE_NON_ASCII(CharType)

#define LONGLP_WHITESPACE_UNICODE(CharType) \
  LONGLP_WHITESPACE_ASCII(CharType)         \
  LONGLP_WHITESPACE_UNICODE_NON_ASCII(CharType)

// Includes Unicode.
BASE_EXPORT constexpr StringViewUTF8 kWhitespaceUTF8 =
  LONGLP_WHITESPACE_UNICODE(UTF8);
BASE_EXPORT constexpr StringViewUTF16 kWhitespaceUTF16 =
  LONGLP_WHITESPACE_UNICODE(UTF16);
BASE_EXPORT constexpr StringViewUTF32 kWhitespaceUTF32 =
  LONGLP_WHITESPACE_UNICODE(UTF32);

// Unicode w/o CR/LF.
BASE_EXPORT constexpr StringViewUTF16 kWhitespaceUTF16NoCRLF =
  LONGLP_WHITESPACE_UNICODE_NO_CR_LF(UTF16);
BASE_EXPORT constexpr StringViewUTF32 kWhitespaceUTF32NoCRLF =
  LONGLP_WHITESPACE_UNICODE_NO_CR_LF(UTF32);
BASE_EXPORT constexpr StringViewUTF8 kWhitespaceUTF8NoCRLF =
  LONGLP_WHITESPACE_UNICODE_NO_CR_LF(UTF8);

BASE_EXPORT constexpr StringViewASCII kWhitespaceASCII =
  LONGLP_WHITESPACE_ASCII(ASCII);

// No unicode.
BASE_EXPORT constexpr StringViewUTF8 kWhitespaceASCIIAsUTF8 =
  LONGLP_WHITESPACE_ASCII(UTF8);
BASE_EXPORT constexpr StringViewUTF16 kWhitespaceASCIIAsUTF16 =
  LONGLP_WHITESPACE_ASCII(UTF16);
BASE_EXPORT constexpr StringViewUTF32 kWhitespaceASCIIAsUTF32 =
  LONGLP_WHITESPACE_ASCII(UTF32);

// https://infra.spec.whatwg.org/#ascii-whitespace
BASE_EXPORT constexpr StringViewASCII kWhitespaceInfraASCII =
  LONGLP_LITERAL_ASCII("\u0009\u000A\u000C\u000D\u0020");

// Null-terminated string representing the UTF-8 byte order mark.
BASE_EXPORT constexpr StringViewUTF8 kUTF8ByteOrderMark =
  LONGLP_LITERAL_UTF8("\u00EF\u00BB\u00BF");

#undef LONGLP_WHITESPACE_ASCII_NO_CR_LF
#undef LONGLP_WHITESPACE_ASCII
#undef LONGLP_WHITESPACE_UNICODE_NON_ASCII
#undef LONGLP_WHITESPACE_UNICODE_NO_CR_LF
#undef LONGLP_WHITESPACE_UNICODE
}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_STRINGS_STRING_UTILS_CONSTANTS_H_
