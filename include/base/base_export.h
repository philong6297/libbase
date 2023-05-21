// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_BASE_BASE_EXPORT_H_
#define LONGLP_BASE_BASE_EXPORT_H_

#include "base/predef.h"

#if defined(LONGLP_OS_WINDOWS)
#  if defined(BASE_IMPLEMENTATION)
#    define BASE_EXPORT __declspec(dllexport)
#  elif defined(BASE_AS_SHARED_LIB)
#    define BASE_EXPORT __declspec(dllimport)
#  endif
#elif defined(LONGLP_OS_LINUX)
#  if defined(BASE_IMPLEMENTATION) || defined(BASE_AS_SHARED_LIB)
#    if defined(LONGLP_COMPILER_GCC) || defined(LONGLP_COMPILER_CLANG)
#      define BASE_EXPORT [[gnu::visibility("default")]]
#    endif
#  endif
#else
#  if !defined(BASE_EXPORT)
#    define BASE_EXPORT
#  endif
#endif

#endif    // LONGLP_BASE_BASE_EXPORT_H_
