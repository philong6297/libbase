// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_STRINGS_TYPEDEFS_H_
#define LONGLP_INCLUDE_BASE_STRINGS_TYPEDEFS_H_

#include <string>
#include <string_view>

#include "base/macros.h"

namespace longlp::base {
// Enforce to use UTF-8 as std::u8string and ASCII as std::string
using UTF8String      = std::u8string;
using UTF16String     = std::u16string;
using UTF32String     = std::u32string;
using ASCIIString     = std::string;

using UTF8StringView  = std::u8string_view;
using UTF16StringView = std::u16string_view;
using UTF32StringView = std::u32string_view;
using ASCIIStringView = std::string_view;

using UTF8Char        = char8_t;
using UTF16Char       = char16_t;
using UTF32Char       = char32_t;
using ASCIIChar       = char;

// Personal taste, C++ string literal is not easily to remember, make macros
// instead
// Usage: LONGLP_..._LITERAL("test123")
#define LONGLP_INTERNAL_UTF8_LITERAL(x) u8##x
#define LONGLP_UTF8_LITERAL(x)          LONGLP_INTERNAL_UTF8_LITERAL(x)

#define LONGLP_INTERNAL_UTF16_LITERAL(x) u##x
#define LONGLP_UTF16_LITERAL(x)          LONGLP_INTERNAL_UTF16_LITERAL(x)

#define LONGLP_INTERNAL_UTF32_LITERAL(x) U##x
#define LONGLP_UTF32_LITERAL(x)          LONGLP_INTERNAL_UTF32_LITERAL(x)

#define LONGLP_INTERNAL_ASCII_LITERAL(x) x
#define LONGLP_ASCII_LITERAL(x)          LONGLP_INTERNAL_ASCII_LITERAL(x)

}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_STRINGS_TYPEDEFS_H_
