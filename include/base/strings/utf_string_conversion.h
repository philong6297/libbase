// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_STRINGS_UTF_STRING_CONVERSION_H_
#define LONGLP_INCLUDE_BASE_STRINGS_UTF_STRING_CONVERSION_H_

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

#include "base/base_export.h"
#include "base/predef.h"
#include "base/strings/typedefs.h"

namespace longlp::base {

// NOLINTBEGIN(*-avoid-c-arrays)

// These convert between UTF-8, -16, -32 and ASCII strings. They are potentially
// slow, so avoid unnecessary conversions. The low-level versions return a
// boolean indicating whether the conversion was 100% valid. In this case, it
// will still do the best it can and put the result in the output buffer. The
// versions that return strings ignore this error and just return the best
// conversion possible.

// UTF32 To Others
BASE_EXPORT auto
UTF32ToUTF8(StringViewUTF32 utf32, StringUTF8& utf8_output) -> bool;
BASE_EXPORT auto
UTF32ToUTF16(StringViewUTF32 utf32, StringUTF16& utf16_output) -> bool;
// Converts to 7-bit ASCII by truncating. The result must be known to be ASCII
// beforehand.
BASE_EXPORT auto
UTF32ToASCII(StringViewUTF32 utf32, StringASCII& ascii_output) -> bool;

// UTF16 To Others
BASE_EXPORT auto
UTF16ToUTF32(StringViewUTF16 utf16, StringUTF32& utf32_output) -> bool;
BASE_EXPORT auto
UTF16ToUTF8(StringViewUTF16 utf16, StringUTF8& utf8_output) -> bool;
// Converts to 7-bit ASCII by truncating. The result must be known to be ASCII
// beforehand.
BASE_EXPORT auto
UTF16ToASCII(StringViewUTF16 utf16, StringASCII& ascii_output) -> bool;

// UTF8 To Others
BASE_EXPORT auto
UTF8ToUTF16(StringViewUTF8 utf8, StringUTF16& utf16_output) -> bool;
BASE_EXPORT auto
UTF8ToUTF32(StringViewUTF8 utf8, StringUTF32& utf32_output) -> bool;
// TODO(longlp, string): utf8-ascii, see
// https://source.chromium.org/chromium/chromium/src/+/main:third_party/libxml/src/encoding.c;l=227;bpv=1;bpt=1?q=UTF8ToAscii&sq=&ss=chromium%2Fchromium%2Fsrc

// ASCII To Others
// This converts an ASCII string, typically a hardcoded constant, to a UTF16
// string.
BASE_EXPORT auto
ASCIIToUTF16(StringViewASCII ascii, StringUTF16& utf16_output) -> bool;
// This converts an ASCII string, typically a hardcoded constant, to a UTF32
// string.
BASE_EXPORT auto
ASCIIToUTF32(StringViewASCII ascii, StringUTF32& utf32_output) -> bool;
// This converts an ASCII string, typically a hardcoded constant, to a UTF8
// string.
BASE_EXPORT auto
ASCIIToUTF8(StringViewASCII ascii, StringUTF8& utf8_output) -> bool;

// The conversion functions in this file should not be used to convert string
// literals. Instead, the corresponding prefixes (e.g. u"" for UTF16 or U"" for
// UTF32) should be used. Deleting the overloads here catches these cases at
// compile time.

template <size_t N>
auto UTF32ToUTF8(const char32_t (&str)[N], StringUTF8& utf8_output)
  -> bool = delete;
template <size_t N>
auto UTF32ToUTF16(const char32_t (&str)[N], StringUTF16& utf16_output)
  -> bool = delete;
template <size_t N>
auto UTF32ToASCII(const char32_t (&str)[N], StringASCII& ascii_output)
  -> bool = delete;

template <size_t N>
auto UTF16ToUTF32(const char16_t (&str)[N], StringUTF32& utf32_output)
  -> bool = delete;
template <size_t N>
auto UTF16ToUTF8(const char16_t (&str)[N], StringUTF8& utf8_output)
  -> bool = delete;
template <size_t N>
auto UTF16ToASCII(const char16_t (&str)[N], StringASCII& ascii_output)
  -> bool = delete;

template <size_t N>
auto UTF8ToUTF16(const char8_t (&str)[N], StringUTF16& utf16_output)
  -> bool = delete;
template <size_t N>
auto UTF8ToUTF32(const char8_t (&str)[N], StringUTF32& utf32_output)
  -> bool = delete;
template <size_t N>
auto UTF8ToASCII(const char8_t (&str)[N], StringASCII& ascii_output)
  -> bool = delete;

template <size_t N>
auto ASCIIToUTF16(const char (&str)[N], StringUTF16& utf16_output)
  -> bool = delete;
template <size_t N>
auto ASCIIToUTF32(const char (&str)[N], StringUTF32& utf32_output)
  -> bool = delete;
template <size_t N>
auto ASCIIToUTF8(const char (&str)[N], StringUTF8& utf8_output)
  -> bool = delete;

// NOLINTEND(*-avoid-c-arrays)
}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_STRINGS_UTF_STRING_CONVERSION_H_
