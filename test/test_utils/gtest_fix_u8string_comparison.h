// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_TEST_TEST_UTILS_GTEST_FIX_U8STRING_COMPARISON_H_
#define LONGLP_TEST_TEST_UTILS_GTEST_FIX_U8STRING_COMPARISON_H_

#include <base/strings/typedefs.h>

namespace longlp::base {
// A fix for Gtest 1.13 cannot print u8string
// https://github.com/google/googletest/commit/92f0d72507801f2d47533bfb2ad8960d93ed0224
// TODO(longlp, gtest): Remove this when we have newer version
void ExpectEQ(StringViewUTF8 str_a, StringViewUTF8 str_b);
auto ToStringASCII(StringViewUTF8 str) -> StringASCII;
}    // namespace longlp::base

#endif    // LONGLP_TEST_TEST_UTILS_GTEST_FIX_U8STRING_COMPARISON_H_
