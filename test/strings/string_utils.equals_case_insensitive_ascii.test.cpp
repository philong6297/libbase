// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {

TEST(StringUtilTest, EqualsCaseInsensitiveASCII) {
#define EXPECT_EQUAL_CASE_INSENSITIVE_ASCII(         \
  bool_suffix,                                       \
  bool_val,                                          \
  literal_a,                                         \
  literal_b)                                         \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_ASCII(literal_a),                 \
    LONGLP_LITERAL_ASCII(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_ASCII(literal_a),                 \
    LONGLP_LITERAL_UTF8(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_ASCII(literal_a),                 \
    LONGLP_LITERAL_UTF16(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_ASCII(literal_a),                 \
    LONGLP_LITERAL_UTF32(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_ASCII(literal_a),               \
      LONGLP_LITERAL_ASCII(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_ASCII(literal_a),               \
      LONGLP_LITERAL_UTF8(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_ASCII(literal_a),               \
      LONGLP_LITERAL_UTF16(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_ASCII(literal_a),               \
      LONGLP_LITERAL_UTF32(literal_b)) == bool_val); \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF8(literal_a),                  \
    LONGLP_LITERAL_ASCII(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF8(literal_a),                  \
    LONGLP_LITERAL_UTF8(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF8(literal_a),                  \
    LONGLP_LITERAL_UTF16(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF8(literal_a),                  \
    LONGLP_LITERAL_UTF32(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF8(literal_a),                \
      LONGLP_LITERAL_ASCII(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF8(literal_a),                \
      LONGLP_LITERAL_UTF8(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF8(literal_a),                \
      LONGLP_LITERAL_UTF16(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF8(literal_a),                \
      LONGLP_LITERAL_UTF32(literal_b)) == bool_val); \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF16(literal_a),                 \
    LONGLP_LITERAL_ASCII(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF16(literal_a),                 \
    LONGLP_LITERAL_UTF8(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF16(literal_a),                 \
    LONGLP_LITERAL_UTF16(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF16(literal_a),                 \
    LONGLP_LITERAL_UTF32(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF16(literal_a),               \
      LONGLP_LITERAL_ASCII(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF16(literal_a),               \
      LONGLP_LITERAL_UTF8(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF16(literal_a),               \
      LONGLP_LITERAL_UTF16(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF16(literal_a),               \
      LONGLP_LITERAL_UTF32(literal_b)) == bool_val); \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF32(literal_a),                 \
    LONGLP_LITERAL_ASCII(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF32(literal_a),                 \
    LONGLP_LITERAL_UTF8(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF32(literal_a),                 \
    LONGLP_LITERAL_UTF16(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_LITERAL_UTF32(literal_a),                 \
    LONGLP_LITERAL_UTF32(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF32(literal_a),               \
      LONGLP_LITERAL_ASCII(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF32(literal_a),               \
      LONGLP_LITERAL_UTF8(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF32(literal_a),               \
      LONGLP_LITERAL_UTF16(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_LITERAL_UTF32(literal_a),               \
      LONGLP_LITERAL_UTF32(literal_b)) == bool_val)

  EXPECT_EQUAL_CASE_INSENSITIVE_ASCII(TRUE, true, "", "");
  EXPECT_EQUAL_CASE_INSENSITIVE_ASCII(TRUE, true, "Asdf", "aSDF");
  EXPECT_EQUAL_CASE_INSENSITIVE_ASCII(FALSE, false, "bsdf", "aSDF");
  EXPECT_EQUAL_CASE_INSENSITIVE_ASCII(FALSE, false, "Asdf", "aSDFz");

#undef EXPECT_EQUAL_CASE_INSENSITIVE_ASCII

  // Non-ASCII bytes are permitted, but they will be compared case-sensitively.
  EXPECT_TRUE(EqualsCaseInsensitiveASCII(u"aaa \xc3\xa4", u8"AAA \xc3\xa4"));
  // comparing signed and unsigned will be false
  EXPECT_FALSE(EqualsCaseInsensitiveASCII("aaa \xc3\xa4", U"AAA \xc3\xa4"));
  EXPECT_FALSE(EqualsCaseInsensitiveASCII("aaa \xc3\x84", "AAA \xc3\xa4"));
}
}    // namespace longlp::base
