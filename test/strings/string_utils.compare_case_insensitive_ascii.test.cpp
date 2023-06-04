// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {

TEST(StringUtilTest, CompareCaseInsensitiveASCII) {
#define EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(          \
  compare_result,                                          \
  literal_a,                                               \
  literal_b)                                               \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_ASCII(literal_a),                     \
      LONGLP_LITERAL_ASCII(literal_b)));                   \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_UTF8(literal_a),                      \
      LONGLP_LITERAL_UTF8(literal_b)));                    \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_UTF16(literal_a),                     \
      LONGLP_LITERAL_UTF16(literal_b)));                   \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_UTF32(literal_a),                     \
      LONGLP_LITERAL_UTF32(literal_b)));                   \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_ASCII(literal_a),                     \
      LONGLP_LITERAL_ASCII(literal_b)) == compare_result); \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_UTF8(literal_a),                      \
      LONGLP_LITERAL_UTF8(literal_b)) == compare_result);  \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_UTF16(literal_a),                     \
      LONGLP_LITERAL_UTF16(literal_b)) == compare_result); \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_LITERAL_UTF32(literal_a),                     \
      LONGLP_LITERAL_UTF32(literal_b)) == compare_result)

  // empty string
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(0, "", "");

  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(0, "Asdf", "aSDf");

  // Differing lengths.
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(-1, "Asdf", "aSDfA");
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(1, "AsdfA", "aSDf");

  // Differing values.
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(-1, "AsdfA", "aSDfb");
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(1, "Asdfb", "aSDfA");

  // Non-ASCII bytes are permitted, but they will be compared case-sensitively.
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(0, "aaa \xc3\xa4", "AAA \xc3\xa4");
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(-1, "AAA \xc3\x84", "aaa \xc3\xa4");
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(1, "aaa \xc3\xa4", "AAA \xc3\x84");

  // ASCII bytes should sort before non-ASCII ones.
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(-1, "a", "\xc3\xa4");
  EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(1, "\xc3\xa4", "a");

#undef EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII
}
}    // namespace longlp::base
