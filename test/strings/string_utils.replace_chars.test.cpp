// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/strings/string_utils.h>

#include <array>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {

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
     {LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8(""),
     false},
     {LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("t"),
     true},
     {LONGLP_LITERAL_UTF8("a"),
     LONGLP_LITERAL_UTF8("b"),
     LONGLP_LITERAL_UTF8("c"),
     LONGLP_LITERAL_UTF8("a"),
     false},
     {LONGLP_LITERAL_UTF8("b"),
     LONGLP_LITERAL_UTF8("b"),
     LONGLP_LITERAL_UTF8("c"),
     LONGLP_LITERAL_UTF8("c"),
     true},
     {LONGLP_LITERAL_UTF8("bob"),
     LONGLP_LITERAL_UTF8("b"),
     LONGLP_LITERAL_UTF8("p"),
     LONGLP_LITERAL_UTF8("pop"),
     true},
     {LONGLP_LITERAL_UTF8("bob"),
     LONGLP_LITERAL_UTF8("o"),
     LONGLP_LITERAL_UTF8("i"),
     LONGLP_LITERAL_UTF8("bib"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8("test"),
     false},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8("!"),
     LONGLP_LITERAL_UTF8("test"),
     false},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("z"),
     LONGLP_LITERAL_UTF8("!"),
     LONGLP_LITERAL_UTF8("test"),
     false},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("e"),
     LONGLP_LITERAL_UTF8("!"),
     LONGLP_LITERAL_UTF8("t!st"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("e"),
     LONGLP_LITERAL_UTF8("!?"),
     LONGLP_LITERAL_UTF8("t!?st"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("ez"),
     LONGLP_LITERAL_UTF8("!"),
     LONGLP_LITERAL_UTF8("t!st"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("zed"),
     LONGLP_LITERAL_UTF8("!?"),
     LONGLP_LITERAL_UTF8("t!?st"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("!?"),
     LONGLP_LITERAL_UTF8("!?es!?"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("et"),
     LONGLP_LITERAL_UTF8("!>"),
     LONGLP_LITERAL_UTF8("!>!>s!>"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("zest"),
     LONGLP_LITERAL_UTF8("!"),
     LONGLP_LITERAL_UTF8("!!!!"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("szt"),
     LONGLP_LITERAL_UTF8("!"),
     LONGLP_LITERAL_UTF8("!e!!"),
     true},
     {LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("testestest"),
     true},
     {LONGLP_LITERAL_UTF8("tetst"),
     LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("test"),
     LONGLP_LITERAL_UTF8("testeteststest"),
     true},
     {LONGLP_LITERAL_UTF8("ttttttt"),
     LONGLP_LITERAL_UTF8("t"),
     LONGLP_LITERAL_UTF8("-"),
     LONGLP_LITERAL_UTF8("-------"),
     true},
     {LONGLP_LITERAL_UTF8("aAaAaAAaAAa"),
     LONGLP_LITERAL_UTF8("A"),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8("aaaaa"),
     true},
     {LONGLP_LITERAL_UTF8("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8(""),
     LONGLP_LITERAL_UTF8(""),
     true},
     {LONGLP_LITERAL_UTF8("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("xxxxxxxxxx"),
     true},
     {LONGLP_LITERAL_UTF8("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("y-"),
     LONGLP_LITERAL_UTF8("y-y-y-y-y-y-y-y-y-y-"),
     true},
     {LONGLP_LITERAL_UTF8("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("xy"),
     LONGLP_LITERAL_UTF8("xyxyxyxyxyxyxyxyxyxy"),
     true},
     {LONGLP_LITERAL_UTF8("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("zyx"),
     LONGLP_LITERAL_UTF8("zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx"),
     true},
     {LONGLP_LITERAL_UTF8("xaxxaxxxaxxxax"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("xy"),
     LONGLP_LITERAL_UTF8("xyaxyxyaxyxyxyaxyxyxyaxy"),
     true},
     {LONGLP_LITERAL_UTF8("-xaxxaxxxaxxxax-"),
     LONGLP_LITERAL_UTF8("x"),
     LONGLP_LITERAL_UTF8("xy"),
     LONGLP_LITERAL_UTF8("-xyaxyxyaxyxyxyaxyxyxyaxy-"),
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
     {LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16(""),
     false},
     {LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("t"),
     true},
     {LONGLP_LITERAL_UTF16("a"),
     LONGLP_LITERAL_UTF16("b"),
     LONGLP_LITERAL_UTF16("c"),
     LONGLP_LITERAL_UTF16("a"),
     false},
     {LONGLP_LITERAL_UTF16("b"),
     LONGLP_LITERAL_UTF16("b"),
     LONGLP_LITERAL_UTF16("c"),
     LONGLP_LITERAL_UTF16("c"),
     true},
     {LONGLP_LITERAL_UTF16("bob"),
     LONGLP_LITERAL_UTF16("b"),
     LONGLP_LITERAL_UTF16("p"),
     LONGLP_LITERAL_UTF16("pop"),
     true},
     {LONGLP_LITERAL_UTF16("bob"),
     LONGLP_LITERAL_UTF16("o"),
     LONGLP_LITERAL_UTF16("i"),
     LONGLP_LITERAL_UTF16("bib"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16("test"),
     false},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16("!"),
     LONGLP_LITERAL_UTF16("test"),
     false},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("z"),
     LONGLP_LITERAL_UTF16("!"),
     LONGLP_LITERAL_UTF16("test"),
     false},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("e"),
     LONGLP_LITERAL_UTF16("!"),
     LONGLP_LITERAL_UTF16("t!st"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("e"),
     LONGLP_LITERAL_UTF16("!?"),
     LONGLP_LITERAL_UTF16("t!?st"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("ez"),
     LONGLP_LITERAL_UTF16("!"),
     LONGLP_LITERAL_UTF16("t!st"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("zed"),
     LONGLP_LITERAL_UTF16("!?"),
     LONGLP_LITERAL_UTF16("t!?st"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("!?"),
     LONGLP_LITERAL_UTF16("!?es!?"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("et"),
     LONGLP_LITERAL_UTF16("!>"),
     LONGLP_LITERAL_UTF16("!>!>s!>"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("zest"),
     LONGLP_LITERAL_UTF16("!"),
     LONGLP_LITERAL_UTF16("!!!!"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("szt"),
     LONGLP_LITERAL_UTF16("!"),
     LONGLP_LITERAL_UTF16("!e!!"),
     true},
     {LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("testestest"),
     true},
     {LONGLP_LITERAL_UTF16("tetst"),
     LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("test"),
     LONGLP_LITERAL_UTF16("testeteststest"),
     true},
     {LONGLP_LITERAL_UTF16("ttttttt"),
     LONGLP_LITERAL_UTF16("t"),
     LONGLP_LITERAL_UTF16("-"),
     LONGLP_LITERAL_UTF16("-------"),
     true},
     {LONGLP_LITERAL_UTF16("aAaAaAAaAAa"),
     LONGLP_LITERAL_UTF16("A"),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16("aaaaa"),
     true},
     {LONGLP_LITERAL_UTF16("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16(""),
     LONGLP_LITERAL_UTF16(""),
     true},
     {LONGLP_LITERAL_UTF16("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("xxxxxxxxxx"),
     true},
     {LONGLP_LITERAL_UTF16("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("y-"),
     LONGLP_LITERAL_UTF16("y-y-y-y-y-y-y-y-y-y-"),
     true},
     {LONGLP_LITERAL_UTF16("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("xy"),
     LONGLP_LITERAL_UTF16("xyxyxyxyxyxyxyxyxyxy"),
     true},
     {LONGLP_LITERAL_UTF16("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("zyx"),
     LONGLP_LITERAL_UTF16("zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx"),
     true},
     {LONGLP_LITERAL_UTF16("xaxxaxxxaxxxax"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("xy"),
     LONGLP_LITERAL_UTF16("xyaxyxyaxyxyxyaxyxyxyaxy"),
     true},
     {LONGLP_LITERAL_UTF16("-xaxxaxxxaxxxax-"),
     LONGLP_LITERAL_UTF16("x"),
     LONGLP_LITERAL_UTF16("xy"),
     LONGLP_LITERAL_UTF16("-xyaxyxyaxyxyxyaxyxyxyaxy-"),
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
     {LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32(""),
     false},
     {LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("t"),
     true},
     {LONGLP_LITERAL_UTF32("a"),
     LONGLP_LITERAL_UTF32("b"),
     LONGLP_LITERAL_UTF32("c"),
     LONGLP_LITERAL_UTF32("a"),
     false},
     {LONGLP_LITERAL_UTF32("b"),
     LONGLP_LITERAL_UTF32("b"),
     LONGLP_LITERAL_UTF32("c"),
     LONGLP_LITERAL_UTF32("c"),
     true},
     {LONGLP_LITERAL_UTF32("bob"),
     LONGLP_LITERAL_UTF32("b"),
     LONGLP_LITERAL_UTF32("p"),
     LONGLP_LITERAL_UTF32("pop"),
     true},
     {LONGLP_LITERAL_UTF32("bob"),
     LONGLP_LITERAL_UTF32("o"),
     LONGLP_LITERAL_UTF32("i"),
     LONGLP_LITERAL_UTF32("bib"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32("test"),
     false},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32("!"),
     LONGLP_LITERAL_UTF32("test"),
     false},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("z"),
     LONGLP_LITERAL_UTF32("!"),
     LONGLP_LITERAL_UTF32("test"),
     false},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("e"),
     LONGLP_LITERAL_UTF32("!"),
     LONGLP_LITERAL_UTF32("t!st"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("e"),
     LONGLP_LITERAL_UTF32("!?"),
     LONGLP_LITERAL_UTF32("t!?st"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("ez"),
     LONGLP_LITERAL_UTF32("!"),
     LONGLP_LITERAL_UTF32("t!st"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("zed"),
     LONGLP_LITERAL_UTF32("!?"),
     LONGLP_LITERAL_UTF32("t!?st"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("!?"),
     LONGLP_LITERAL_UTF32("!?es!?"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("et"),
     LONGLP_LITERAL_UTF32("!>"),
     LONGLP_LITERAL_UTF32("!>!>s!>"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("zest"),
     LONGLP_LITERAL_UTF32("!"),
     LONGLP_LITERAL_UTF32("!!!!"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("szt"),
     LONGLP_LITERAL_UTF32("!"),
     LONGLP_LITERAL_UTF32("!e!!"),
     true},
     {LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("testestest"),
     true},
     {LONGLP_LITERAL_UTF32("tetst"),
     LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("test"),
     LONGLP_LITERAL_UTF32("testeteststest"),
     true},
     {LONGLP_LITERAL_UTF32("ttttttt"),
     LONGLP_LITERAL_UTF32("t"),
     LONGLP_LITERAL_UTF32("-"),
     LONGLP_LITERAL_UTF32("-------"),
     true},
     {LONGLP_LITERAL_UTF32("aAaAaAAaAAa"),
     LONGLP_LITERAL_UTF32("A"),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32("aaaaa"),
     true},
     {LONGLP_LITERAL_UTF32("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32(""),
     LONGLP_LITERAL_UTF32(""),
     true},
     {LONGLP_LITERAL_UTF32("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("xxxxxxxxxx"),
     true},
     {LONGLP_LITERAL_UTF32("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("y-"),
     LONGLP_LITERAL_UTF32("y-y-y-y-y-y-y-y-y-y-"),
     true},
     {LONGLP_LITERAL_UTF32("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("xy"),
     LONGLP_LITERAL_UTF32("xyxyxyxyxyxyxyxyxyxy"),
     true},
     {LONGLP_LITERAL_UTF32("xxxxxxxxxx"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("zyx"),
     LONGLP_LITERAL_UTF32("zyxzyxzyxzyxzyxzyxzyxzyxzyxzyx"),
     true},
     {LONGLP_LITERAL_UTF32("xaxxaxxxaxxxax"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("xy"),
     LONGLP_LITERAL_UTF32("xyaxyxyaxyxyxyaxyxyxyaxy"),
     true},
     {LONGLP_LITERAL_UTF32("-xaxxaxxxaxxxax-"),
     LONGLP_LITERAL_UTF32("x"),
     LONGLP_LITERAL_UTF32("xy"),
     LONGLP_LITERAL_UTF32("-xyaxyxyaxyxyxyaxyxyxyaxy-"),
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
