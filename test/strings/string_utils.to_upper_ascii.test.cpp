// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {

TEST(StringUtilTest, ToUpperASCII) {
  EXPECT_EQ(LONGLP_LITERAL_ASCII('C'), ToUpperASCII(LONGLP_LITERAL_ASCII('C')));
  EXPECT_EQ(LONGLP_LITERAL_ASCII('C'), ToUpperASCII(LONGLP_LITERAL_ASCII('c')));
  EXPECT_EQ(LONGLP_LITERAL_ASCII('2'), ToUpperASCII(LONGLP_LITERAL_ASCII('2')));

  EXPECT_EQ(LONGLP_LITERAL_UTF8('C'), ToUpperASCII(LONGLP_LITERAL_UTF8('C')));
  EXPECT_EQ(LONGLP_LITERAL_UTF8('C'), ToUpperASCII(LONGLP_LITERAL_UTF8('c')));
  EXPECT_EQ(LONGLP_LITERAL_UTF8('2'), ToUpperASCII(LONGLP_LITERAL_UTF8('2')));

  EXPECT_EQ(LONGLP_LITERAL_UTF16('C'), ToUpperASCII(LONGLP_LITERAL_UTF16('C')));
  EXPECT_EQ(LONGLP_LITERAL_UTF16('C'), ToUpperASCII(LONGLP_LITERAL_UTF16('c')));
  EXPECT_EQ(LONGLP_LITERAL_UTF16('2'), ToUpperASCII(LONGLP_LITERAL_UTF16('2')));

  EXPECT_EQ(LONGLP_LITERAL_UTF32('C'), ToUpperASCII(LONGLP_LITERAL_UTF32('C')));
  EXPECT_EQ(LONGLP_LITERAL_UTF32('C'), ToUpperASCII(LONGLP_LITERAL_UTF32('c')));
  EXPECT_EQ(LONGLP_LITERAL_UTF32('2'), ToUpperASCII(LONGLP_LITERAL_UTF32('2')));

  EXPECT_EQ(
    LONGLP_LITERAL_ASCII("CC2"),
    ToUpperASCII(LONGLP_LITERAL_ASCII("Cc2")));
  ExpectEQ(
    LONGLP_LITERAL_UTF8("CC2"),
    ToUpperASCII(LONGLP_LITERAL_UTF8("Cc2")));
  EXPECT_EQ(
    LONGLP_LITERAL_UTF16("CC2"),
    ToUpperASCII(LONGLP_LITERAL_UTF16("Cc2")));
  EXPECT_EQ(
    LONGLP_LITERAL_UTF32("CC2"),
    ToUpperASCII(LONGLP_LITERAL_UTF32("Cc2")));

  // Non-ASCII characters are unmodified. U+00C4 is LATIN CAPITAL LETTER A WITH
  // DIAERESIS.
  EXPECT_EQ(
    LONGLP_LITERAL_ASCII('\x00c4'),
    ToUpperASCII(LONGLP_LITERAL_ASCII('\x00c4')));
  EXPECT_EQ(
    LONGLP_LITERAL_UTF8('\x00c4'),
    ToUpperASCII(LONGLP_LITERAL_UTF8('\x00c4')));
  EXPECT_EQ(
    LONGLP_LITERAL_UTF16('\x00c4'),
    ToUpperASCII(LONGLP_LITERAL_UTF16('\x00c4')));
  EXPECT_EQ(
    LONGLP_LITERAL_UTF32('\x00c4'),
    ToUpperASCII(LONGLP_LITERAL_UTF32('\x00c4')));
}
}    // namespace longlp::base
