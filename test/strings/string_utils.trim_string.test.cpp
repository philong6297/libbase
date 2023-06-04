// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>
#include <vector>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {

namespace {}    // namespace

TEST(StringUtilTest, TrimWhitespace) {
// clang-format off
#define GENERATE_TRIM_CASE(CharType)                              \
  struct TrimCase {                                               \
    StringView##CharType input;                                   \
    TrimPositions positions;                                      \
    StringView##CharType output;                                  \
    TrimPositions return_value;                                   \
  };                                                              \
  const std::vector<TrimCase> kTrimCases = {                      \
    {LONGLP_LITERAL_##CharType(" Test Something "),               \
      TrimPositions::kTrimLeading,                                \
      LONGLP_LITERAL_##CharType("Test Something "),               \
      TrimPositions::kTrimLeading},                               \
     {LONGLP_LITERAL_##CharType(" Test Something "),              \
      TrimPositions::kTrimTrailing,                               \
      LONGLP_LITERAL_##CharType(" Test Something"),               \
      TrimPositions::kTrimTrailing},                              \
     {LONGLP_LITERAL_##CharType(" Test Something "),              \
      TrimPositions::kTrimAll,                                    \
      LONGLP_LITERAL_##CharType("Test Something"),                \
      TrimPositions::kTrimAll},                                   \
     {LONGLP_LITERAL_##CharType("Test Something"),                \
      TrimPositions::kTrimAll,                                    \
      LONGLP_LITERAL_##CharType("Test Something"),                \
      TrimPositions::kTrimNone},                                  \
     {LONGLP_LITERAL_##CharType(""),                              \
      TrimPositions::kTrimAll,                                    \
      LONGLP_LITERAL_##CharType(""),                              \
      TrimPositions::kTrimNone},                                  \
     {LONGLP_LITERAL_##CharType("  "),                            \
      TrimPositions::kTrimLeading,                                \
      LONGLP_LITERAL_##CharType(""),                              \
      TrimPositions::kTrimLeading},                               \
     {LONGLP_LITERAL_##CharType("  "),                            \
      TrimPositions::kTrimTrailing,                               \
      LONGLP_LITERAL_##CharType(""),                              \
      TrimPositions::kTrimTrailing},                              \
     {LONGLP_LITERAL_##CharType("  "),                            \
      TrimPositions::kTrimAll,                                    \
      LONGLP_LITERAL_##CharType(""),                              \
      TrimPositions::kTrimAll},                                   \
     {LONGLP_LITERAL_##CharType("\t\rTest String\n"),             \
      TrimPositions::kTrimAll,                                    \
      LONGLP_LITERAL_##CharType("Test String"),                   \
      TrimPositions::kTrimAll},                                   \
     {LONGLP_LITERAL_##CharType("\u2002Test String\u00A0\u3000"), \
      TrimPositions::kTrimAll,                                    \
      LONGLP_LITERAL_##CharType("Test String"),                   \
      TrimPositions::kTrimAll}}
#define TEST_TRIM_CASE(CharType)                                               \
  String##CharType output; /* Allow contents to carry over to next testcase */ \
  for (const auto& value : kTrimCases) {                                       \
    EXPECT_EQ(                                                                 \
      value.return_value,                                                      \
      TrimWhitespace(value.input, value.positions, output));                   \
    EXPECT_EQ(value.output, output);                                           \
  }                                                                            \
  /* Test that TrimWhitespace() can take the same string for input and output  \
   */                                                                          \
  output = LONGLP_LITERAL_##CharType("  This is a test \r\n");                 \
  EXPECT_EQ(                                                                   \
    TrimPositions::kTrimAll,                                                   \
    TrimWhitespace(output, TrimPositions::kTrimAll, output));                  \
  EXPECT_EQ(LONGLP_LITERAL_##CharType("This is a test"), output);              \
  /* Once more, but with a string of whitespace */                             \
  output = LONGLP_LITERAL_##CharType("  \r\n");                                \
  EXPECT_EQ(                                                                   \
    TrimPositions::kTrimAll,                                                   \
    TrimWhitespace(output, TrimPositions::kTrimAll, output));                  \
  EXPECT_EQ(String##CharType(), output)

  // clang-format on

  {
    GENERATE_TRIM_CASE(UTF16);
    TEST_TRIM_CASE(UTF16);
  }
  {
    GENERATE_TRIM_CASE(UTF32);
    TEST_TRIM_CASE(UTF32);
  }
  {
    GENERATE_TRIM_CASE(UTF8);
    StringUTF8 output;    // Allow contents to carry over to next testcase
    for (const auto& value : kTrimCases) {
      EXPECT_EQ(
        value.return_value,
        TrimWhitespace(value.input, value.positions, output));
      ExpectEQ(value.output, output);
    }
    // Test that TrimWhitespace() can take the same string for input and output
    output = LONGLP_LITERAL_UTF8("  This is a test \r\n");
    EXPECT_EQ(
      TrimPositions::kTrimAll,
      TrimWhitespace(output, TrimPositions::kTrimAll, output));
    ExpectEQ(LONGLP_LITERAL_UTF8("This is a test"), output);
    // Once more, but with a string of whitespace
    output = LONGLP_LITERAL_UTF8("  \r\n");
    EXPECT_EQ(
      TrimPositions::kTrimAll,
      TrimWhitespace(output, TrimPositions::kTrimAll, output));
    ExpectEQ(StringUTF8(), output);
  }
}

TEST(StringUtilTest, TrimWhitespaceASCII) {
// clang-format off
#define GENERATE_TRIM_CASE_ASCII(CharType)                      \
  struct TrimCase {                                       \
    StringView##CharType input;                           \
    TrimPositions positions;                              \
    StringView##CharType output;                          \
    TrimPositions return_value;                           \
  };                                                      \
  const std::vector<TrimCase> kTrimCases = {              \
    {LONGLP_LITERAL_##CharType(" Test Something "),       \
     TrimPositions::kTrimLeading,                         \
     LONGLP_LITERAL_##CharType("Test Something "),        \
     TrimPositions::kTrimLeading},                        \
    {LONGLP_LITERAL_##CharType(" Test Something "),       \
     TrimPositions::kTrimTrailing,                        \
     LONGLP_LITERAL_##CharType(" Test Something"),        \
     TrimPositions::kTrimTrailing},                       \
    {LONGLP_LITERAL_##CharType(" Test Something "),       \
     TrimPositions::kTrimAll,                             \
     LONGLP_LITERAL_##CharType("Test Something"),         \
     TrimPositions::kTrimAll},                            \
    {LONGLP_LITERAL_##CharType("Test Something"),         \
     TrimPositions::kTrimAll,                             \
     LONGLP_LITERAL_##CharType("Test Something"),         \
     TrimPositions::kTrimNone},                           \
    {LONGLP_LITERAL_##CharType(""),                       \
     TrimPositions::kTrimAll,                             \
     LONGLP_LITERAL_##CharType(""),                       \
     TrimPositions::kTrimNone},                           \
    {LONGLP_LITERAL_##CharType("  "),                     \
     TrimPositions::kTrimLeading,                         \
     LONGLP_LITERAL_##CharType(""),                       \
     TrimPositions::kTrimLeading},                        \
    {LONGLP_LITERAL_##CharType("  "),                     \
     TrimPositions::kTrimTrailing,                        \
     LONGLP_LITERAL_##CharType(""),                       \
     TrimPositions::kTrimTrailing},                       \
    {LONGLP_LITERAL_##CharType("  "),                     \
     TrimPositions::kTrimAll,                             \
     LONGLP_LITERAL_##CharType(""),                       \
     TrimPositions::kTrimAll},                            \
    {LONGLP_LITERAL_##CharType("\t\rTest String\n"),      \
     TrimPositions::kTrimAll,                             \
     LONGLP_LITERAL_##CharType("Test String"),            \
     TrimPositions::kTrimAll}                             \
  }
#define TEST_TRIM_CASE_ASCII(CharType)                                         \
  String##CharType output;                                                     \
  for (const auto& value : kTrimCases) {                                       \
    EXPECT_EQ(                                                                 \
      value.return_value,                                                      \
      TrimWhitespaceASCII(value.input, value.positions, output));              \
    EXPECT_EQ(value.output, output);                                           \
  }

  // clang-format on

  {
    GENERATE_TRIM_CASE_ASCII(ASCII);
    TEST_TRIM_CASE_ASCII(ASCII)
  }
  {
    GENERATE_TRIM_CASE_ASCII(UTF16);
    TEST_TRIM_CASE_ASCII(UTF16)
  }
  {
    GENERATE_TRIM_CASE_ASCII(UTF32);
    TEST_TRIM_CASE_ASCII(UTF32)
  }
  {
    GENERATE_TRIM_CASE_ASCII(UTF8);
    StringUTF8 output;
    for (const auto& value : kTrimCases) {
      EXPECT_EQ(
        value.return_value,
        TrimWhitespace(value.input, value.positions, output));
      ExpectEQ(value.output, output);
    }
  }
}

}    // namespace longlp::base
