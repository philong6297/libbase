// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_COMPILER_SPECIFIC_H_
#define LONGLP_INCLUDE_BASE_COMPILER_SPECIFIC_H_

#include "base/macros.h"
#include "base/predef.h"

#if defined(LONGLP_COMPILER_CLANG)
#  define LONGLP_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#  define LONGLP_DIAGNOSTIC_POP  _Pragma("clang diagnostic pop")
#  define LONGLP_CLANG_DIAGNOSTIC_IGNORED(str) \
    _Pragma(LONGLP_EXPAND_THEN_STRINGIZE(clang diagnostic ignored str))
#  define LONGLP_GCC_DIAGNOSTIC_IGNORED(str)
#  define LONGLP_MSVC_DIAGNOSTIC_IGNORED(num)
#elif defined(LONGLP_COMPILER_GCC)
#  define LONGLP_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#  define LONGLP_DIAGNOSTIC_POP  _Pragma("GCC diagnostic pop")
#  define LONGLP_CLANG_DIAGNOSTIC_IGNORED(str)
#  define LONGLP_GCC_DIAGNOSTIC_IGNORED(str) \
    _Pragma(LONGLP_EXPAND_THEN_STRINGIZE(GCC diagnostic ignored str))
#  define LONGLP_MSVC_DIAGNOSTIC_IGNORED(num)
#elif defined(LONGLP_COMPILER_MSVC)
#  define LONGLP_DIAGNOSTIC_PUSH _Pragma("warning(push)")
#  define LONGLP_DIAGNOSTIC_POP  _Pragma("warning(pop)")
#  define LONGLP_CLANG_DIAGNOSTIC_IGNORED(str)
#  define LONGLP_GCC_DIAGNOSTIC_IGNORED(str)
#  define LONGLP_MSVC_DIAGNOSTIC_IGNORED(num) \
    _Pragma(LONGLP_EXPAND_THEN_STRINGIZE(warning(disable : num)))
#endif

// This is a wrapper around `__has_cpp_attribute`, which can be used to test for
// the presence of an attribute. In case the compiler does not support this
// macro it will simply evaluate to 0.
//
// References:
// https://wg21.link/sd6#testing-for-the-presence-of-an-attribute-__has_cpp_attribute
// https://wg21.link/cpp.cond#:__has_cpp_attribute
#if defined(__has_cpp_attribute)
#  define LONGLP_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#  define LONGLP_HAS_CPP_ATTRIBUTE(x) 0
#endif

// A wrapper around `__has_attribute`, similar to HAS_CPP_ATTRIBUTE.
#if defined(__has_attribute)
#  define LONGLP_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#  define LONGLP_HAS_ATTRIBUTE(x) 0
#endif

#if defined(LONGLP_COMPILER_CLANG) && defined(NDEBUG) && \
  LONGLP_HAS_ATTRIBUTE(always_inline)
#  define LONGLP_ALWAYS_INLINE [[clang::always_inline]] inline
#elif defined(LONGLP_COMPILER_GCC) && defined(NDEBUG) && \
  LONGLP_HAS_ATTRIBUTE(always_inline)
#  define LONGLP_ALWAYS_INLINE inline __attribute__((__always_inline__))
#elif defined(LONGLP_COMPILER_MSVC) && defined(NDEBUG)
#  define LONGLP_ALWAYS_INLINE __forceinline
#else
#  define LONGLP_ALWAYS_INLINE inline
#endif

#if defined(LONGLP_COMPILER_CLANG)
#  define LONGLP_GSL_OWNER   [[gsl::Owner]]
#  define LONGLP_GSL_POINTER [[gsl::Pointer]]
#else
#  define LONGLP_GSL_OWNER
#  define LONGLP_GSL_POINTER
#endif

#endif    // LONGLP_INCLUDE_BASE_COMPILER_SPECIFIC_H_
