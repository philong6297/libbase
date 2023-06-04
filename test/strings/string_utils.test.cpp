// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {
TEST(StringUtilTest, ToLowerASCII) {
  EXPECT_EQ(LONGLP_ASCII_LITERAL('c'), ToLowerASCII(LONGLP_ASCII_LITERAL('C')));
  EXPECT_EQ(LONGLP_ASCII_LITERAL('c'), ToLowerASCII(LONGLP_ASCII_LITERAL('c')));
  EXPECT_EQ(LONGLP_ASCII_LITERAL('2'), ToLowerASCII(LONGLP_ASCII_LITERAL('2')));

  EXPECT_EQ(LONGLP_UTF8_LITERAL('c'), ToLowerASCII(LONGLP_UTF8_LITERAL('C')));
  EXPECT_EQ(LONGLP_UTF8_LITERAL('c'), ToLowerASCII(LONGLP_UTF8_LITERAL('c')));
  EXPECT_EQ(LONGLP_UTF8_LITERAL('2'), ToLowerASCII(LONGLP_UTF8_LITERAL('2')));

  EXPECT_EQ(LONGLP_UTF16_LITERAL('c'), ToLowerASCII(LONGLP_UTF16_LITERAL('C')));
  EXPECT_EQ(LONGLP_UTF16_LITERAL('c'), ToLowerASCII(LONGLP_UTF16_LITERAL('c')));
  EXPECT_EQ(LONGLP_UTF16_LITERAL('2'), ToLowerASCII(LONGLP_UTF16_LITERAL('2')));

  EXPECT_EQ(LONGLP_UTF32_LITERAL('c'), ToLowerASCII(LONGLP_UTF32_LITERAL('C')));
  EXPECT_EQ(LONGLP_UTF32_LITERAL('c'), ToLowerASCII(LONGLP_UTF32_LITERAL('c')));
  EXPECT_EQ(LONGLP_UTF32_LITERAL('2'), ToLowerASCII(LONGLP_UTF32_LITERAL('2')));

  EXPECT_EQ(
    LONGLP_ASCII_LITERAL("cc2"),
    ToLowerASCII(LONGLP_ASCII_LITERAL("Cc2")));
  ExpectEQ(
    LONGLP_UTF8_LITERAL("cc2"),
    ToLowerASCII(LONGLP_UTF8_LITERAL("Cc2")));
  EXPECT_EQ(
    LONGLP_UTF16_LITERAL("cc2"),
    ToLowerASCII(LONGLP_UTF16_LITERAL("Cc2")));
  EXPECT_EQ(
    LONGLP_UTF32_LITERAL("cc2"),
    ToLowerASCII(LONGLP_UTF32_LITERAL("Cc2")));

  // Non-ASCII characters are unmodified. U+00C4 is LATIN CAPITAL LETTER A WITH
  // DIAERESIS.
  EXPECT_EQ(
    LONGLP_ASCII_LITERAL('\x00c4'),
    ToLowerASCII(LONGLP_ASCII_LITERAL('\x00c4')));
  EXPECT_EQ(
    LONGLP_UTF8_LITERAL('\x00c4'),
    ToLowerASCII(LONGLP_UTF8_LITERAL('\x00c4')));
  EXPECT_EQ(
    LONGLP_UTF16_LITERAL('\x00c4'),
    ToLowerASCII(LONGLP_UTF8_LITERAL('\x00c4')));
  EXPECT_EQ(
    LONGLP_UTF32_LITERAL('\x00c4'),
    ToLowerASCII(LONGLP_UTF8_LITERAL('\x00c4')));
}

TEST(StringUtilTest, ToUpperASCII) {
  EXPECT_EQ(LONGLP_ASCII_LITERAL('C'), ToUpperASCII(LONGLP_ASCII_LITERAL('C')));
  EXPECT_EQ(LONGLP_ASCII_LITERAL('C'), ToUpperASCII(LONGLP_ASCII_LITERAL('c')));
  EXPECT_EQ(LONGLP_ASCII_LITERAL('2'), ToUpperASCII(LONGLP_ASCII_LITERAL('2')));

  EXPECT_EQ(LONGLP_UTF8_LITERAL('C'), ToUpperASCII(LONGLP_UTF8_LITERAL('C')));
  EXPECT_EQ(LONGLP_UTF8_LITERAL('C'), ToUpperASCII(LONGLP_UTF8_LITERAL('c')));
  EXPECT_EQ(LONGLP_UTF8_LITERAL('2'), ToUpperASCII(LONGLP_UTF8_LITERAL('2')));

  EXPECT_EQ(LONGLP_UTF16_LITERAL('C'), ToUpperASCII(LONGLP_UTF16_LITERAL('C')));
  EXPECT_EQ(LONGLP_UTF16_LITERAL('C'), ToUpperASCII(LONGLP_UTF16_LITERAL('c')));
  EXPECT_EQ(LONGLP_UTF16_LITERAL('2'), ToUpperASCII(LONGLP_UTF16_LITERAL('2')));

  EXPECT_EQ(LONGLP_UTF32_LITERAL('C'), ToUpperASCII(LONGLP_UTF32_LITERAL('C')));
  EXPECT_EQ(LONGLP_UTF32_LITERAL('C'), ToUpperASCII(LONGLP_UTF32_LITERAL('c')));
  EXPECT_EQ(LONGLP_UTF32_LITERAL('2'), ToUpperASCII(LONGLP_UTF32_LITERAL('2')));

  EXPECT_EQ(
    LONGLP_ASCII_LITERAL("CC2"),
    ToUpperASCII(LONGLP_ASCII_LITERAL("Cc2")));
  ExpectEQ(
    LONGLP_UTF8_LITERAL("CC2"),
    ToUpperASCII(LONGLP_UTF8_LITERAL("Cc2")));
  EXPECT_EQ(
    LONGLP_UTF16_LITERAL("CC2"),
    ToUpperASCII(LONGLP_UTF16_LITERAL("Cc2")));
  EXPECT_EQ(
    LONGLP_UTF32_LITERAL("CC2"),
    ToUpperASCII(LONGLP_UTF32_LITERAL("Cc2")));

  // Non-ASCII characters are unmodified. U+00C4 is LATIN CAPITAL LETTER A WITH
  // DIAERESIS.
  EXPECT_EQ(
    LONGLP_ASCII_LITERAL('\x00c4'),
    ToUpperASCII(LONGLP_ASCII_LITERAL('\x00c4')));
  EXPECT_EQ(
    LONGLP_UTF8_LITERAL('\x00c4'),
    ToUpperASCII(LONGLP_UTF8_LITERAL('\x00c4')));
  EXPECT_EQ(
    LONGLP_UTF16_LITERAL('\x00c4'),
    ToUpperASCII(LONGLP_UTF16_LITERAL('\x00c4')));
  EXPECT_EQ(
    LONGLP_UTF32_LITERAL('\x00c4'),
    ToUpperASCII(LONGLP_UTF32_LITERAL('\x00c4')));
}

TEST(StringUtilTest, CompareCaseInsensitiveASCII) {
#define EXPECT_EQ_COMPARE_CASE_INSENSITIVE_ASCII(          \
  compare_result,                                          \
  literal_a,                                               \
  literal_b)                                               \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_ASCII_LITERAL(literal_a),                     \
      LONGLP_ASCII_LITERAL(literal_b)));                   \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_UTF8_LITERAL(literal_a),                      \
      LONGLP_UTF8_LITERAL(literal_b)));                    \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_UTF16_LITERAL(literal_a),                     \
      LONGLP_UTF16_LITERAL(literal_b)));                   \
  EXPECT_EQ(                                               \
    compare_result,                                        \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_UTF32_LITERAL(literal_a),                     \
      LONGLP_UTF32_LITERAL(literal_b)));                   \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_ASCII_LITERAL(literal_a),                     \
      LONGLP_ASCII_LITERAL(literal_b)) == compare_result); \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_UTF8_LITERAL(literal_a),                      \
      LONGLP_UTF8_LITERAL(literal_b)) == compare_result);  \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_UTF16_LITERAL(literal_a),                     \
      LONGLP_UTF16_LITERAL(literal_b)) == compare_result); \
  static_assert(                                           \
    CompareCaseInsensitiveASCII(                           \
      LONGLP_UTF32_LITERAL(literal_a),                     \
      LONGLP_UTF32_LITERAL(literal_b)) == compare_result)

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

TEST(StringUtilTest, EqualsCaseInsensitiveASCII) {
#define EXPECT_EQUAL_CASE_INSENSITIVE_ASCII(         \
  bool_suffix,                                       \
  bool_val,                                          \
  literal_a,                                         \
  literal_b)                                         \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_ASCII_LITERAL(literal_a),                 \
    LONGLP_ASCII_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_ASCII_LITERAL(literal_a),                 \
    LONGLP_UTF8_LITERAL(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_ASCII_LITERAL(literal_a),                 \
    LONGLP_UTF16_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_ASCII_LITERAL(literal_a),                 \
    LONGLP_UTF32_LITERAL(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_ASCII_LITERAL(literal_a),               \
      LONGLP_ASCII_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_ASCII_LITERAL(literal_a),               \
      LONGLP_UTF8_LITERAL(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_ASCII_LITERAL(literal_a),               \
      LONGLP_UTF16_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_ASCII_LITERAL(literal_a),               \
      LONGLP_UTF32_LITERAL(literal_b)) == bool_val); \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF8_LITERAL(literal_a),                  \
    LONGLP_ASCII_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF8_LITERAL(literal_a),                  \
    LONGLP_UTF8_LITERAL(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF8_LITERAL(literal_a),                  \
    LONGLP_UTF16_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF8_LITERAL(literal_a),                  \
    LONGLP_UTF32_LITERAL(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF8_LITERAL(literal_a),                \
      LONGLP_ASCII_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF8_LITERAL(literal_a),                \
      LONGLP_UTF8_LITERAL(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF8_LITERAL(literal_a),                \
      LONGLP_UTF16_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF8_LITERAL(literal_a),                \
      LONGLP_UTF32_LITERAL(literal_b)) == bool_val); \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF16_LITERAL(literal_a),                 \
    LONGLP_ASCII_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF16_LITERAL(literal_a),                 \
    LONGLP_UTF8_LITERAL(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF16_LITERAL(literal_a),                 \
    LONGLP_UTF16_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF16_LITERAL(literal_a),                 \
    LONGLP_UTF32_LITERAL(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF16_LITERAL(literal_a),               \
      LONGLP_ASCII_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF16_LITERAL(literal_a),               \
      LONGLP_UTF8_LITERAL(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF16_LITERAL(literal_a),               \
      LONGLP_UTF16_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF16_LITERAL(literal_a),               \
      LONGLP_UTF32_LITERAL(literal_b)) == bool_val); \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF32_LITERAL(literal_a),                 \
    LONGLP_ASCII_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF32_LITERAL(literal_a),                 \
    LONGLP_UTF8_LITERAL(literal_b)));                \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF32_LITERAL(literal_a),                 \
    LONGLP_UTF16_LITERAL(literal_b)));               \
  EXPECT_##bool_suffix(EqualsCaseInsensitiveASCII(   \
    LONGLP_UTF32_LITERAL(literal_a),                 \
    LONGLP_UTF32_LITERAL(literal_b)));               \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF32_LITERAL(literal_a),               \
      LONGLP_ASCII_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF32_LITERAL(literal_a),               \
      LONGLP_UTF8_LITERAL(literal_b)) == bool_val);  \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF32_LITERAL(literal_a),               \
      LONGLP_UTF16_LITERAL(literal_b)) == bool_val); \
  static_assert(                                     \
    EqualsCaseInsensitiveASCII(                      \
      LONGLP_UTF32_LITERAL(literal_a),               \
      LONGLP_UTF32_LITERAL(literal_b)) == bool_val)

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

TEST(StringUtilTest, RemoveChars) {
  {
    constexpr StringViewASCII kRemoveChars = LONGLP_ASCII_LITERAL("-/+*");
    StringASCII input                      = LONGLP_ASCII_LITERAL("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_ASCII_LITERAL("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_ASCII_LITERAL("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(StringASCII(), input);
  }
  {
    constexpr StringViewUTF8 kRemoveChars = LONGLP_UTF8_LITERAL("-/+*");
    StringUTF8 input                      = LONGLP_UTF8_LITERAL("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    ExpectEQ(LONGLP_UTF8_LITERAL("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    ExpectEQ(LONGLP_UTF8_LITERAL("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    ExpectEQ(StringUTF8(), input);
  }
  {
    constexpr StringViewUTF16 kRemoveChars = LONGLP_UTF16_LITERAL("-/+*");
    StringUTF16 input                      = LONGLP_UTF16_LITERAL("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_UTF16_LITERAL("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_UTF16_LITERAL("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(StringUTF16(), input);
  }
  {
    constexpr StringViewUTF32 kRemoveChars = LONGLP_UTF32_LITERAL("-/+*");
    StringUTF32 input                      = LONGLP_UTF32_LITERAL("A-+bc/d!*");
    EXPECT_TRUE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_UTF32_LITERAL("Abcd!"), input);

    // No characters match kRemoveChars.
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(LONGLP_UTF32_LITERAL("Abcd!"), input);

    // Empty string.
    input.clear();
    EXPECT_FALSE(RemoveChars(input, kRemoveChars, input));
    EXPECT_EQ(StringUTF32(), input);
  }
}

TEST(StringUtilTest, ReplaceCharsASCII) {
  struct TestData {
    StringViewASCII input;
    StringViewASCII replace_chars;
    StringViewASCII replace_with;
    StringViewASCII output;
    bool result;
  };

  constexpr std::array<TestData, 28> kCases = {
    {
     {"", "", "", "", false},
     {"t", "t", "t", "t", true},
     {"a", "b", "c", "a", false},
     {"b", "b", "c", "c", true},
     {"bob", "b", "p", "pop", true},
     {"bob", "o", "i", "bib", true},
     {"test", "", "", "test", false},
     {"test", "", "!", "test", false},
     {"test", "z", "!", "test", false},
     {"test", "e", "!", "t!st", true},
     {"test", "e", "!?", "t!?st", true},
     {"test", "ez", "!", "t!st", true},
     {"test", "zed", "!?", "t!?st", true},
     {"test", "t", "!?", "!?es!?", true},
     {"test", "et", "!>", "!>!>s!>", true},
     {"test", "zest", "!", "!!!!", true},
     {"test", "szt", "!", "!e!!", true},
     {"test", "t", "test", "testestest", true},
     {"tetst", "t", "test", "testeteststest", true},
     {"ttttttt", "t", "-", "-------", true},
     {"aAaAaAAaAAa", "A", "", "aaaaa", true},
     {"xxxxxxxxxx", "x", "", "", true},
     {"xxxxxxxxxx", "x", "x", "xxxxxxxxxx", true},
     {"xxxxxxxxxx", "x", "y-", "y-y-y-y-y-y-y-y-y-y-", true},
     {"xxxxxxxxxx", "x", "xy", "xyxyxyxyxyxyxyxyxyxy", true},
     {"xxxxxxxxxx", "x", "zyx", "zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx", true},
     {"xaxxaxxxaxxxax", "x", "xy", "xyaxyxyaxyxyxyaxyxyxyaxy", true},
     {"-xaxxaxxxaxxxax-", "x", "xy", "-xyaxyxyaxyxyxyaxyxyxyaxy-", true},
     }
  };

  for (const TestData& scenario : kCases) {
    // Test with separate output and input vars.
    std::string output;
    bool result = ReplaceChars(
      scenario.input,
      scenario.replace_chars,
      scenario.replace_with,
      output);
    EXPECT_EQ(scenario.result, result) << scenario.input;
    EXPECT_EQ(scenario.output, output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of limited capacity.
    std::string input_output(scenario.input);
    input_output.shrink_to_fit();
    bool result = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result) << scenario.input;
    EXPECT_EQ(scenario.output, input_output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of ample capacity; should
    // not realloc.
    std::string input_output(scenario.input);
    input_output.reserve(scenario.output.size() * 2);
    const void* original_buffer = input_output.data();
    bool result                 = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result) << scenario.input;
    EXPECT_EQ(scenario.output, input_output);
    EXPECT_EQ(original_buffer, input_output.data());
  }
}

TEST(StringUtilTest, ReplaceCharsUTF8) {
  struct TestData {
    StringViewUTF8 input;
    StringViewUTF8 replace_chars;
    StringViewUTF8 replace_with;
    StringViewUTF8 output;
    bool result;
  };

  constexpr std::array<TestData, 28> kCases = {
    {
     {LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL(""),
     false},
     {LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("t"),
     true},
     {LONGLP_UTF8_LITERAL("a"),
     LONGLP_UTF8_LITERAL("b"),
     LONGLP_UTF8_LITERAL("c"),
     LONGLP_UTF8_LITERAL("a"),
     false},
     {LONGLP_UTF8_LITERAL("b"),
     LONGLP_UTF8_LITERAL("b"),
     LONGLP_UTF8_LITERAL("c"),
     LONGLP_UTF8_LITERAL("c"),
     true},
     {LONGLP_UTF8_LITERAL("bob"),
     LONGLP_UTF8_LITERAL("b"),
     LONGLP_UTF8_LITERAL("p"),
     LONGLP_UTF8_LITERAL("pop"),
     true},
     {LONGLP_UTF8_LITERAL("bob"),
     LONGLP_UTF8_LITERAL("o"),
     LONGLP_UTF8_LITERAL("i"),
     LONGLP_UTF8_LITERAL("bib"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL("test"),
     false},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL("!"),
     LONGLP_UTF8_LITERAL("test"),
     false},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("z"),
     LONGLP_UTF8_LITERAL("!"),
     LONGLP_UTF8_LITERAL("test"),
     false},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("e"),
     LONGLP_UTF8_LITERAL("!"),
     LONGLP_UTF8_LITERAL("t!st"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("e"),
     LONGLP_UTF8_LITERAL("!?"),
     LONGLP_UTF8_LITERAL("t!?st"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("ez"),
     LONGLP_UTF8_LITERAL("!"),
     LONGLP_UTF8_LITERAL("t!st"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("zed"),
     LONGLP_UTF8_LITERAL("!?"),
     LONGLP_UTF8_LITERAL("t!?st"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("!?"),
     LONGLP_UTF8_LITERAL("!?es!?"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("et"),
     LONGLP_UTF8_LITERAL("!>"),
     LONGLP_UTF8_LITERAL("!>!>s!>"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("zest"),
     LONGLP_UTF8_LITERAL("!"),
     LONGLP_UTF8_LITERAL("!!!!"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("szt"),
     LONGLP_UTF8_LITERAL("!"),
     LONGLP_UTF8_LITERAL("!e!!"),
     true},
     {LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("testestest"),
     true},
     {LONGLP_UTF8_LITERAL("tetst"),
     LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("test"),
     LONGLP_UTF8_LITERAL("testeteststest"),
     true},
     {LONGLP_UTF8_LITERAL("ttttttt"),
     LONGLP_UTF8_LITERAL("t"),
     LONGLP_UTF8_LITERAL("-"),
     LONGLP_UTF8_LITERAL("-------"),
     true},
     {LONGLP_UTF8_LITERAL("aAaAaAAaAAa"),
     LONGLP_UTF8_LITERAL("A"),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL("aaaaa"),
     true},
     {LONGLP_UTF8_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL(""),
     LONGLP_UTF8_LITERAL(""),
     true},
     {LONGLP_UTF8_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("xxxxxxxxxx"),
     true},
     {LONGLP_UTF8_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("y-"),
     LONGLP_UTF8_LITERAL("y-y-y-y-y-y-y-y-y-y-"),
     true},
     {LONGLP_UTF8_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("xy"),
     LONGLP_UTF8_LITERAL("xyxyxyxyxyxyxyxyxyxy"),
     true},
     {LONGLP_UTF8_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("zyx"),
     LONGLP_UTF8_LITERAL("zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx"),
     true},
     {LONGLP_UTF8_LITERAL("xaxxaxxxaxxxax"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("xy"),
     LONGLP_UTF8_LITERAL("xyaxyxyaxyxyxyaxyxyxyaxy"),
     true},
     {LONGLP_UTF8_LITERAL("-xaxxaxxxaxxxax-"),
     LONGLP_UTF8_LITERAL("x"),
     LONGLP_UTF8_LITERAL("xy"),
     LONGLP_UTF8_LITERAL("-xyaxyxyaxyxyxyaxyxyxyaxy-"),
     true},
     }
  };

  for (const TestData& scenario : kCases) {
    // Test with separate output and input vars.
    StringUTF8 output;
    bool result = ReplaceChars(
      scenario.input,
      scenario.replace_chars,
      scenario.replace_with,
      output);
    EXPECT_EQ(scenario.result, result) << ToStringASCII(scenario.input);
    ExpectEQ(scenario.output, output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of limited capacity.
    StringUTF8 input_output(scenario.input);
    input_output.shrink_to_fit();
    bool result = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result) << ToStringASCII(scenario.input);
    ExpectEQ(scenario.output, input_output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of ample capacity; should
    // not realloc.
    StringUTF8 input_output(scenario.input);
    input_output.reserve(scenario.output.size() * 2);
    const void* original_buffer = input_output.data();
    bool result                 = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result) << ToStringASCII(scenario.input);
    ExpectEQ(scenario.output, input_output);
    EXPECT_EQ(original_buffer, input_output.data());
  }
}

TEST(StringUtilTest, ReplaceCharsUTF16) {
  struct TestData {
    StringViewUTF16 input;
    StringViewUTF16 replace_chars;
    StringViewUTF16 replace_with;
    StringViewUTF16 output;
    bool result;
  };

  constexpr std::array<TestData, 28> kCases = {
    {
     {LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL(""),
     false},
     {LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("t"),
     true},
     {LONGLP_UTF16_LITERAL("a"),
     LONGLP_UTF16_LITERAL("b"),
     LONGLP_UTF16_LITERAL("c"),
     LONGLP_UTF16_LITERAL("a"),
     false},
     {LONGLP_UTF16_LITERAL("b"),
     LONGLP_UTF16_LITERAL("b"),
     LONGLP_UTF16_LITERAL("c"),
     LONGLP_UTF16_LITERAL("c"),
     true},
     {LONGLP_UTF16_LITERAL("bob"),
     LONGLP_UTF16_LITERAL("b"),
     LONGLP_UTF16_LITERAL("p"),
     LONGLP_UTF16_LITERAL("pop"),
     true},
     {LONGLP_UTF16_LITERAL("bob"),
     LONGLP_UTF16_LITERAL("o"),
     LONGLP_UTF16_LITERAL("i"),
     LONGLP_UTF16_LITERAL("bib"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL("test"),
     false},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL("!"),
     LONGLP_UTF16_LITERAL("test"),
     false},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("z"),
     LONGLP_UTF16_LITERAL("!"),
     LONGLP_UTF16_LITERAL("test"),
     false},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("e"),
     LONGLP_UTF16_LITERAL("!"),
     LONGLP_UTF16_LITERAL("t!st"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("e"),
     LONGLP_UTF16_LITERAL("!?"),
     LONGLP_UTF16_LITERAL("t!?st"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("ez"),
     LONGLP_UTF16_LITERAL("!"),
     LONGLP_UTF16_LITERAL("t!st"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("zed"),
     LONGLP_UTF16_LITERAL("!?"),
     LONGLP_UTF16_LITERAL("t!?st"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("!?"),
     LONGLP_UTF16_LITERAL("!?es!?"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("et"),
     LONGLP_UTF16_LITERAL("!>"),
     LONGLP_UTF16_LITERAL("!>!>s!>"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("zest"),
     LONGLP_UTF16_LITERAL("!"),
     LONGLP_UTF16_LITERAL("!!!!"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("szt"),
     LONGLP_UTF16_LITERAL("!"),
     LONGLP_UTF16_LITERAL("!e!!"),
     true},
     {LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("testestest"),
     true},
     {LONGLP_UTF16_LITERAL("tetst"),
     LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("test"),
     LONGLP_UTF16_LITERAL("testeteststest"),
     true},
     {LONGLP_UTF16_LITERAL("ttttttt"),
     LONGLP_UTF16_LITERAL("t"),
     LONGLP_UTF16_LITERAL("-"),
     LONGLP_UTF16_LITERAL("-------"),
     true},
     {LONGLP_UTF16_LITERAL("aAaAaAAaAAa"),
     LONGLP_UTF16_LITERAL("A"),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL("aaaaa"),
     true},
     {LONGLP_UTF16_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL(""),
     LONGLP_UTF16_LITERAL(""),
     true},
     {LONGLP_UTF16_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("xxxxxxxxxx"),
     true},
     {LONGLP_UTF16_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("y-"),
     LONGLP_UTF16_LITERAL("y-y-y-y-y-y-y-y-y-y-"),
     true},
     {LONGLP_UTF16_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("xy"),
     LONGLP_UTF16_LITERAL("xyxyxyxyxyxyxyxyxyxy"),
     true},
     {LONGLP_UTF16_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("zyx"),
     LONGLP_UTF16_LITERAL("zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx"),
     true},
     {LONGLP_UTF16_LITERAL("xaxxaxxxaxxxax"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("xy"),
     LONGLP_UTF16_LITERAL("xyaxyxyaxyxyxyaxyxyxyaxy"),
     true},
     {LONGLP_UTF16_LITERAL("-xaxxaxxxaxxxax-"),
     LONGLP_UTF16_LITERAL("x"),
     LONGLP_UTF16_LITERAL("xy"),
     LONGLP_UTF16_LITERAL("-xyaxyxyaxyxyxyaxyxyxyaxy-"),
     true},
     }
  };

  for (const TestData& scenario : kCases) {
    // Test with separate output and input vars.
    StringUTF16 output;
    bool result = ReplaceChars(
      scenario.input,
      scenario.replace_chars,
      scenario.replace_with,
      output);
    EXPECT_EQ(scenario.result, result)
      << testing::PrintToString(scenario.input);
    EXPECT_EQ(scenario.output, output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of limited capacity.
    StringUTF16 input_output(scenario.input);
    input_output.shrink_to_fit();
    bool result = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result)
      << testing::PrintToString(scenario.input);
    EXPECT_EQ(scenario.output, input_output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of ample capacity; should
    // not realloc.
    StringUTF16 input_output(scenario.input);
    input_output.reserve(scenario.output.size() * 2);
    const void* original_buffer = input_output.data();
    bool result                 = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result)
      << testing::PrintToString(scenario.input);
    EXPECT_EQ(scenario.output, input_output);
    EXPECT_EQ(original_buffer, input_output.data());
  }
}

TEST(StringUtilTest, ReplaceCharsUTF32) {
  struct TestData {
    StringViewUTF32 input;
    StringViewUTF32 replace_chars;
    StringViewUTF32 replace_with;
    StringViewUTF32 output;
    bool result;
  };

  constexpr std::array<TestData, 28> kCases = {
    {
     {LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL(""),
     false},
     {LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("t"),
     true},
     {LONGLP_UTF32_LITERAL("a"),
     LONGLP_UTF32_LITERAL("b"),
     LONGLP_UTF32_LITERAL("c"),
     LONGLP_UTF32_LITERAL("a"),
     false},
     {LONGLP_UTF32_LITERAL("b"),
     LONGLP_UTF32_LITERAL("b"),
     LONGLP_UTF32_LITERAL("c"),
     LONGLP_UTF32_LITERAL("c"),
     true},
     {LONGLP_UTF32_LITERAL("bob"),
     LONGLP_UTF32_LITERAL("b"),
     LONGLP_UTF32_LITERAL("p"),
     LONGLP_UTF32_LITERAL("pop"),
     true},
     {LONGLP_UTF32_LITERAL("bob"),
     LONGLP_UTF32_LITERAL("o"),
     LONGLP_UTF32_LITERAL("i"),
     LONGLP_UTF32_LITERAL("bib"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL("test"),
     false},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL("!"),
     LONGLP_UTF32_LITERAL("test"),
     false},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("z"),
     LONGLP_UTF32_LITERAL("!"),
     LONGLP_UTF32_LITERAL("test"),
     false},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("e"),
     LONGLP_UTF32_LITERAL("!"),
     LONGLP_UTF32_LITERAL("t!st"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("e"),
     LONGLP_UTF32_LITERAL("!?"),
     LONGLP_UTF32_LITERAL("t!?st"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("ez"),
     LONGLP_UTF32_LITERAL("!"),
     LONGLP_UTF32_LITERAL("t!st"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("zed"),
     LONGLP_UTF32_LITERAL("!?"),
     LONGLP_UTF32_LITERAL("t!?st"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("!?"),
     LONGLP_UTF32_LITERAL("!?es!?"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("et"),
     LONGLP_UTF32_LITERAL("!>"),
     LONGLP_UTF32_LITERAL("!>!>s!>"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("zest"),
     LONGLP_UTF32_LITERAL("!"),
     LONGLP_UTF32_LITERAL("!!!!"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("szt"),
     LONGLP_UTF32_LITERAL("!"),
     LONGLP_UTF32_LITERAL("!e!!"),
     true},
     {LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("testestest"),
     true},
     {LONGLP_UTF32_LITERAL("tetst"),
     LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("test"),
     LONGLP_UTF32_LITERAL("testeteststest"),
     true},
     {LONGLP_UTF32_LITERAL("ttttttt"),
     LONGLP_UTF32_LITERAL("t"),
     LONGLP_UTF32_LITERAL("-"),
     LONGLP_UTF32_LITERAL("-------"),
     true},
     {LONGLP_UTF32_LITERAL("aAaAaAAaAAa"),
     LONGLP_UTF32_LITERAL("A"),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL("aaaaa"),
     true},
     {LONGLP_UTF32_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL(""),
     LONGLP_UTF32_LITERAL(""),
     true},
     {LONGLP_UTF32_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("xxxxxxxxxx"),
     true},
     {LONGLP_UTF32_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("y-"),
     LONGLP_UTF32_LITERAL("y-y-y-y-y-y-y-y-y-y-"),
     true},
     {LONGLP_UTF32_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("xy"),
     LONGLP_UTF32_LITERAL("xyxyxyxyxyxyxyxyxyxy"),
     true},
     {LONGLP_UTF32_LITERAL("xxxxxxxxxx"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("zyx"),
     LONGLP_UTF32_LITERAL("zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx"),
     true},
     {LONGLP_UTF32_LITERAL("xaxxaxxxaxxxax"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("xy"),
     LONGLP_UTF32_LITERAL("xyaxyxyaxyxyxyaxyxyxyaxy"),
     true},
     {LONGLP_UTF32_LITERAL("-xaxxaxxxaxxxax-"),
     LONGLP_UTF32_LITERAL("x"),
     LONGLP_UTF32_LITERAL("xy"),
     LONGLP_UTF32_LITERAL("-xyaxyxyaxyxyxyaxyxyxyaxy-"),
     true},
     }
  };

  for (const TestData& scenario : kCases) {
    // Test with separate output and input vars.
    StringUTF32 output;
    bool result = ReplaceChars(
      scenario.input,
      scenario.replace_chars,
      scenario.replace_with,
      output);
    EXPECT_EQ(scenario.result, result)
      << testing::PrintToString(scenario.input);
    EXPECT_EQ(scenario.output, output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of limited capacity.
    StringUTF32 input_output(scenario.input);
    input_output.shrink_to_fit();
    bool result = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result)
      << testing::PrintToString(scenario.input);
    EXPECT_EQ(scenario.output, input_output);
  }

  for (const TestData& scenario : kCases) {
    // Test with an input/output var of ample capacity; should
    // not realloc.
    StringUTF32 input_output(scenario.input);
    input_output.reserve(scenario.output.size() * 2);
    const void* original_buffer = input_output.data();
    bool result                 = ReplaceChars(
      input_output,
      scenario.replace_chars,
      scenario.replace_with,
      input_output);
    EXPECT_EQ(scenario.result, result)
      << testing::PrintToString(scenario.input);
    EXPECT_EQ(scenario.output, input_output);
    EXPECT_EQ(original_buffer, input_output.data());
  }
}

}    // namespace longlp::base
