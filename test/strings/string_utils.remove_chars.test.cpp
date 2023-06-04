// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {

TEST(StringUtilTest, RemoveChars) {
  {
    constexpr StringViewASCII kRemoveChars = LONGLP_LITERAL_ASCII("-/+*");
    StringASCII input                      = LONGLP_LITERAL_ASCII("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_LITERAL_ASCII("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_LITERAL_ASCII("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(StringASCII(), input);
  }
  {
    constexpr StringViewUTF8 kRemoveChars = LONGLP_LITERAL_UTF8("-/+*");
    StringUTF8 input                      = LONGLP_LITERAL_UTF8("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    ExpectEQ(LONGLP_LITERAL_UTF8("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    ExpectEQ(LONGLP_LITERAL_UTF8("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    ExpectEQ(StringUTF8(), input);
  }
  {
    constexpr StringViewUTF16 kRemoveChars = LONGLP_LITERAL_UTF16("-/+*");
    StringUTF16 input                      = LONGLP_LITERAL_UTF16("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_LITERAL_UTF16("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_LITERAL_UTF16("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(StringUTF16(), input);
  }
  {
    constexpr StringViewUTF32 kRemoveChars = LONGLP_LITERAL_UTF32("-/+*");
    StringUTF32 input                      = LONGLP_LITERAL_UTF32("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_LITERAL_UTF32("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_LITERAL_UTF32("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(StringUTF32(), input);
  }
}
}    // namespace longlp::base
