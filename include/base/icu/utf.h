// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_ICU_UTF_H_
#define LONGLP_INCLUDE_BASE_ICU_UTF_H_

// This file has the relevant components from ICU copied to handle basic
// UTF8/16/32 conversions. Components are copied from utf8.h
// and utf16.h into utf.h. The functions and types have been put in the
// "icu" namespace.

#include <cstdint>

#include <unicode/utf16.h>
#include <unicode/utf8.h>

#include "base/compiler_specific.h"
#include "base/types/strong_alias.h"

namespace longlp::base::icu {
namespace internal {

  LONGLP_DIAGNOSTIC_PUSH
  LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wunsafe-buffer-usage")

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf8_8h.html#aa2298b48749d9f45772c8f5a6885464a
  LONGLP_ALWAYS_INLINE constexpr auto kU8MaxLength  = U8_MAX_LENGTH;
  LONGLP_ALWAYS_INLINE constexpr auto kU16MaxLength = U16_MAX_LENGTH;

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf8_8h.html#a57f3e5429ae4edb27a42367c627aa482
  // The length can be negative for a NUL-terminated string
  LONGLP_ALWAYS_INLINE void U8Next(
    const uint8_t* src,
    int32_t& offset,
    const int32_t length,
    UChar32& codepoint) {
    LONGLP_DIAGNOSTIC_PUSH
    LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wsign-compare")

    U8_NEXT(src, offset, length, codepoint);

    LONGLP_DIAGNOSTIC_POP
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#ac25b589c0c9b60160d357770fad39cea
  LONGLP_ALWAYS_INLINE constexpr auto
  U16IsSurrogate(const char16_t utf16_c) -> bool {
    return U16_IS_SURROGATE(utf16_c);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#a6e141a548138e8c24822d219b7e06cb4
  LONGLP_ALWAYS_INLINE constexpr auto
  U16IsSurrogateLead(const char16_t utf16_c) {
    return U16_IS_SURROGATE_LEAD(utf16_c);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#afe8d9f450b9297897f018c2f23eb0724
  LONGLP_ALWAYS_INLINE constexpr auto
  U16IsTrail(const char16_t utf16_c) -> bool {
    return U16_IS_TRAIL(utf16_c);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#ac1deffbf1956d9fe696129515e88f006
  LONGLP_ALWAYS_INLINE constexpr auto
  U16GetSupplementary(const char16_t lead, const char16_t trail) -> UChar32 {
    return U16_GET_SUPPLEMENTARY(lead, trail);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf8_8h.html#a154f04764da5af41729c4df6bf9e09f3
  LONGLP_ALWAYS_INLINE void
  U8AppendUnsafe(uint8_t* src, size_t& offset, const UChar32 codepoint) {
    U8_APPEND_UNSAFE(src, offset, codepoint);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#add0a383d49e1ca81e2920d25883a56a9
  LONGLP_ALWAYS_INLINE constexpr auto U16Length(const UChar32 codepoint) {
    return U16_LENGTH(codepoint);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#aea8253343c96066779cd3383080cafa8
  LONGLP_ALWAYS_INLINE void
  U16AppendUnsafe(UChar* src, size_t& offset, const UChar32 codepoint) {
    U16_APPEND_UNSAFE(src, offset, codepoint);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#a35f04f1f6e7f0965a66b5268eec29b99
  LONGLP_ALWAYS_INLINE auto U16IsSingle(char16_t codeunit) -> bool {
    return U16_IS_SINGLE(codeunit);
  }

  // See
  // https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/utf16_8h.html#ace839ae31a801fd9c53fa67c5f8b9144
  LONGLP_ALWAYS_INLINE auto U16IsLead(char16_t codeunit) -> bool {
    return U16_IS_LEAD(codeunit);
  }

  LONGLP_DIAGNOSTIC_POP

// Any used macro should be undefined to prevent the usage of these macros in
// our codebase
#undef U8_MAX_LENGTH
#undef U8_NEXT
#undef U8_APPEND_UNSAFE

#undef U16_MAX_LENGTH
#undef U16_IS_SURROGATE
#undef U16_IS_SURROGATE_LEAD
#undef U16_IS_TRAIL
#undef U16_GET_SUPPLEMENTARY
#undef U16_LENGTH
#undef U16_APPEND_UNSAFE
#undef U16_IS_SINGLE
#undef U16_IS_LEAD
}    // namespace internal

using CodePoint = StrongAlias<class LongLPCodePoint, UChar32>;
}    // namespace longlp::base::icu

#endif    // LONGLP_INCLUDE_BASE_ICU_UTF_H_
