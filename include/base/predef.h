// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_PREDEF_H_
#define LONGLP_INCLUDE_BASE_PREDEF_H_

// This file defines a set of compiler, architecture, operating system,
// library, and other version numbers from the information it can gather of C,
// C++ predefined macros or those defined in generally available headers.
// Use Boost.Predef is good enough for now
//
// Currently we are targeting to support:
// - Compiler: MSCV, GCC and Clang
// - OS: Windows, Linux
// Any other will result in an error
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
#  define LONGLP_OS_WINDOWS
#else
#  undef LONGLP_OS_WINDOWS
#endif

#if defined(BOOST_OS_LINUX_AVAILABLE)
#  define LONGLP_OS_LINUX
#else
#  undef LONGLP_OS_LINUX
#endif

#if !defined(LONGLP_OS_LINUX) && !defined(LONGLP_OS_WINDOWS)
#  error "Currently, libbase is only support Windows and Linux"
#endif

#if defined(BOOST_COMP_CLANG_AVAILABLE)
#  define LONGLP_COMPILER_CLANG
#else
#  undef LONGLP_COMPILER_CLANG
#endif

// __GNUC__ and __GNUG__ were intended to indicate the GNU compilers. However,
// they're also defined by LLVM/Clang to indicate compatibility
#if defined(BOOST_COMP_GNUC_AVAILABLE) && !defined(LONGLP_COMPILER_CLANG)
#  define LONGLP_COMPILER_GCC
#else
#  undef LONGLP_COMPILER_GCC
#endif

// _MSC_VER and related macros were intended to indicate MSVC compilers.
// However, they're also defined by LLVM/Clang on Windows to indicate
// compatibility
#if defined(BOOST_COMP_MSVC_AVAILABLE) && !defined(LONGLP_COMPILER_CLANG)
#  define LONGLP_COMPILER_MSVC
#else
#  undef LONGLP_COMPILER_MSVC
#endif

#if !defined(LONGLP_COMPILER_GCC) && !defined(LONGLP_COMPILER_CLANG) && \
  !defined(LONGLP_COMPILER_MSVC)
#  error \
    "Currently, libbase is only support compiler GNU/GCC, LLVM/Clang and MSVC"
#endif

#if defined(BOOST_ARCH_X86_AVAILABLE)
#  define LONGLP_ARCH_CPU_X86_FAMILY
#else
#  undef LONGLP_ARCH_CPU_X86_FAMILY
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#  define LONGLP_ARCH_CPU_ARM64
#else
#  undef LONGLP_ARCH_CPU_ARM64
#endif

#endif    // LONGLP_INCLUDE_BASE_PREDEF_H_
