// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/immediate_crash.h>
#include <base/predef.h>

#if defined(LONGLP_OS_WINDOWS)
#  if defined(IMMEDIATE_CRASH_TEST_HELPER_IMPLEMENTATION)
#    define IMMEDIATE_CRASH_TEST_HELPER_EXPORT __declspec(dllexport)
#  elif defined(BUILD_SHARED_LIBS)
#    define IMMEDIATE_CRASH_TEST_HELPER_EXPORT __declspec(dllimport)
#  endif
#elif defined(LONGLP_OS_LINUX)
#  if defined(IMMEDIATE_CRASH_TEST_HELPER_IMPLEMENTATION) || \
    defined(BUILD_SHARED_LIBS)
#    if defined(LONGLP_COMPILER_GCC) || defined(LONGLP_COMPILER_CLANG)
#      define IMMEDIATE_CRASH_TEST_HELPER_EXPORT [[gnu::visibility("default")]]
#    endif
#  endif
#else
#  if !defined(IMMEDIATE_CRASH_TEST_HELPER_EXPORT)
#    define IMMEDIATE_CRASH_TEST_HELPER_EXPORT
#  endif
#endif

extern "C" {

  IMMEDIATE_CRASH_TEST_HELPER_EXPORT auto TestFunction1(int x, int y) -> int {
    if (x < 1) {
      longlp::base::ImmediateCrash();
    }
    if (y < 1) {
      longlp::base::ImmediateCrash();
    }
    return x + y;
  }

  IMMEDIATE_CRASH_TEST_HELPER_EXPORT auto TestFunction2(int x, int y) -> int {
    if (x < 2) {
      longlp::base::ImmediateCrash();
    }
    if (y < 2) {
      longlp::base::ImmediateCrash();
    }
    return x * y;
  }

}    // extern "C"
