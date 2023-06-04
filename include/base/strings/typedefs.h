// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_STRINGS_TYPEDEFS_H_
#define LONGLP_INCLUDE_BASE_STRINGS_TYPEDEFS_H_

#include <concepts>
#include <string>
#include <string_view>

#include "base/macros.h"

namespace longlp::base {
// Enforce to use UTF-8 as std::u8string and ASCII as std::string
using StringUTF8      = std::u8string;
using StringUTF16     = std::u16string;
using StringUTF32     = std::u32string;
using StringASCII     = std::string;

using StringViewUTF8  = std::u8string_view;
using StringViewUTF16 = std::u16string_view;
using StringViewUTF32 = std::u32string_view;
using StringViewASCII = std::string_view;

using CharUTF8        = char8_t;
using CharUTF16       = char16_t;
using CharUTF32       = char32_t;
using CharASCII       = char;

// follow C++ named requirements: CharTraits
// https://en.cppreference.com/w/cpp/named_req/CharTraits
// NOLINTBEGIN(readability-identifier-length)
template <typename T>
concept CharTraits = requires(
  T c,
  T d,
  const T* p,
  const T* q,
  T* s,
  size_t n,
  size_t i,
  size_t j,
  std::char_traits<T>::int_type e,
  std::char_traits<T>::int_type f,
  std::char_traits<T>::pos_type pos,
  std::char_traits<T>::state_type state,
  T& r) {
  requires std::same_as<typename std::char_traits<T>::char_type, T>;
  typename std::char_traits<T>::int_type;
  typename std::char_traits<T>::off_type;
  typename std::char_traits<T>::pos_type;
  typename std::char_traits<T>::state_type;

  { std::char_traits<T>::eq(c, d) } -> std::same_as<bool>;

  { std::char_traits<T>::lt(c, d) } -> std::same_as<bool>;

  { std::char_traits<T>::compare(p, q, n) } -> std::same_as<int>;

  { std::char_traits<T>::length(p) } -> std::same_as<size_t>;

  { std::char_traits<T>::find(p, n, c) } -> std::same_as<const T*>;

  { std::char_traits<T>::move(s, p, n) } -> std::same_as<T*>;

  { std::char_traits<T>::copy(s, p, n) } -> std::same_as<T*>;

  { std::char_traits<T>::assign(r, d) } -> std::same_as<void>;

  { std::char_traits<T>::assign(s, n, c) } -> std::same_as<T*>;

  {
    std::char_traits<T>::not_eof(e)
  } -> std::same_as<typename std::char_traits<T>::int_type>;

  { std::char_traits<T>::to_char_type(e) } -> std::same_as<T>;
  {
    std::char_traits<T>::to_int_type(c)
  } -> std::same_as<typename std::char_traits<T>::int_type>;

  { std::char_traits<T>::eq_int_type(e, f) } -> std::same_as<bool>;

  {
    std::char_traits<T>::eof()
  } -> std::same_as<typename std::char_traits<T>::int_type>;
};
// NOLINTEND(readability-identifier-length)

// Personal taste, C++ string literal is not easily to remember, make macros
// instead
// Usage: LONGLP_..._LITERAL("test123")
#define LONGLP_INTERNAL_LITERAL_UTF8(x) u8##x
#define LONGLP_LITERAL_UTF8(x)          LONGLP_INTERNAL_LITERAL_UTF8(x)

#define LONGLP_INTERNAL_LITERAL_UTF16(x) u##x
#define LONGLP_LITERAL_UTF16(x)          LONGLP_INTERNAL_LITERAL_UTF16(x)

#define LONGLP_INTERNAL_LITERAL_UTF32(x) U##x
#define LONGLP_LITERAL_UTF32(x)          LONGLP_INTERNAL_LITERAL_UTF32(x)

#define LONGLP_INTERNAL_LITERAL_ASCII(x) x
#define LONGLP_LITERAL_ASCII(x)          LONGLP_INTERNAL_LITERAL_ASCII(x)
}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_STRINGS_TYPEDEFS_H_
