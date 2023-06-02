// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

// Integrated from
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf16tst.c
//
// Replace same macro functions to their corresponding function in
// base/icu/utf.h for minimum effort of writting test cases
//
// Remove U_HIDE_OBSOLETE_UTF_OLD_H parts

#include <base/icu/utf.h>

#include <algorithm>
#include <array>

#include <fmt/printf.h>
#include <gtest/gtest.h>

namespace longlp::base::icu {
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while, cppcoreguidelines-macro-usage)
namespace {
  using internal::U16AppendUnsafe;
  // using internal::U16GetSupplementary;    // No test found
  using internal::U16IsSurrogate;
  // using internal::U16IsSurrogateLead;    // No test found
  using internal::U16IsTrail;
  using internal::U16Length;
}    // namespace

// Integrated from TestCodeUnitValues() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf16tst.c
// and TestCodeUnit() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/cucdtst.c
TEST(UTF16Test, TestCodeUnitValues) {
  // clang-format off
  static constexpr std::array<uint16_t, 13> kCodeUnit = {
    0x0000,
    0xe065,
    0x20ac,
    0xd7ff,
    0xd800,
    0xd841,
    0xd905,
    0xdbff,
    0xdc00,
    0xdc02,
    0xddee,
    0xdfff,
    0
  };
  // clang-format on

  // Keep U16_IS_SINGLE, U16_IS_LEAD for testing purpose
  for (size_t i = 0U; i < kCodeUnit.size(); ++i) {
    const auto c = kCodeUnit.at(i);
    if (i < 4) {
      EXPECT_TRUE(
        U16_IS_SINGLE(c) && !U16_IS_LEAD(c) && !U16IsTrail(c) &&
        !U16IsSurrogate(c))
        << fmt::sprintf("ERROR: %x is a single character\n", c);
    }
    if (i >= 4 && i < 8) {
      EXPECT_TRUE(
        !U16_IS_SINGLE(c) && U16_IS_LEAD(c) && !U16IsTrail(c) &&
        U16IsSurrogate(c))
        << fmt::sprintf("ERROR: %x is a first surrogate\n", c);
    }
    if (i >= 8 && i < 12) {
      EXPECT_TRUE(
        !U16_IS_SINGLE(c) && !U16_IS_LEAD(c) && U16IsTrail(c) &&
        U16IsSurrogate(c))
        << fmt::sprintf("ERROR: %x is a second surrogate\n", c);
    }
  }
}

// Integrated from TestCharLength() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf16tst.c
TEST(UTF16Test, TestCharLength) {
  // clang-format off
    static constexpr std::array<uint32_t, 20> kCodePointAndLength = {
      // length   codepoint
        1, 0x0061,
        1, 0xe065,
        1, 0x20ac,
        2, 0x20402,
        2, 0x23456,
        2, 0x24506,
        2, 0x20402,
        2, 0x10402,
        1, 0xd7ff,
        1, 0xe000
    };
  // clang-format on
  for (size_t i = 0U; i < kCodePointAndLength.size(); i += 2) {
    const auto codepoint = static_cast<UChar32>(kCodePointAndLength.at(i + 1));
    const auto actual_length   = U16Length(codepoint);
    const auto expected_length = kCodePointAndLength.at(i);

    EXPECT_EQ(expected_length, actual_length) << fmt::sprintf(
      "The no: of code units for %lx:- Expected: %d Got: %d\n",
      codepoint,
      expected_length,
      actual_length);
  }
}

// Integrated from TestCodePoint() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/cucdtst.c
TEST(UTF16Test, TestCodePoint) {
  // clang-format off
  const std::array<UChar32,32> kCodePoint = {
    // surrogate, notvalid(codepoint), not a UnicodeChar, not Error
    0xd800,
    0xdbff,
    0xdc00,
    0xdfff,
    0xdc04,
    0xd821,
    // not a surrogate, valid, isUnicodeChar , not Error
    0x20ac,
    0xd7ff,
    0xe000,
    0xe123,
    0x0061,
    0xe065,
    0x20402,
    0x24506,
    0x23456,
    0x20402,
    0x10402,
    0x23456,
    // not a surrogate, not valid, isUnicodeChar, isError
    0x0015,
    0x009f,
    // not a surrogate, not valid, not isUnicodeChar, isError
    0xffff,
    0xfffe,
  };
  // clang-format on

  // Keep U_IS_SURROGATE for testing purpose
  for (size_t i = 0U; i < kCodePoint.size(); ++i) {
    const UChar32 c    = kCodePoint.at(i);
    const auto utf16_c = static_cast<char16_t>(c);
    if (i < 6) {
      EXPECT_TRUE(U_IS_SURROGATE(c) && U16IsSurrogate(utf16_c))
        << fmt::sprintf("ERROR: isSurrogate() failed for U+%04x\n", c);
      // The next part in TestCodePoint is not intended to test
      // U16_IS_SURROGATE, skip for now
    }
    else if (i >= 6 && i < 18) {
      EXPECT_TRUE(!U_IS_SURROGATE(c) && !U16IsSurrogate(utf16_c))
        << fmt::sprintf("ERROR: isSurrogate() failed for U+%04x\n", c);
      // The next part in TestCodePoint is not intended to test
      // U16_IS_SURROGATE, skip for now
    }
    else if (i >= 18 && i < 20) {
      EXPECT_TRUE(!U_IS_SURROGATE(c) && !U16IsSurrogate(utf16_c))
        << fmt::sprintf("ERROR: isSurrogate() failed for U+%04x\n", c);
      // The next part in TestCodePoint is not intended to test
      // U16_IS_SURROGATE, skip for now
    }
    else if (i >= 18 && i < kCodePoint.size()) {
      EXPECT_TRUE(!U_IS_SURROGATE(c) && !U16IsSurrogate(utf16_c))
        << fmt::sprintf("ERROR: isSurrogate() failed for U+%04x\n", c);
      // The next part in TestCodePoint is not intended to test
      // U16_IS_SURROGATE, skip for now
    }
  }

  // The next part in TestCodePoint is not intended to test U16_IS_SURROGATE,
  // skip for now
}

// Integrated from TestAppend in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf16tst.c
TEST(UTF16Test, TestAppend) {
  // clang-format off
    static const std::array<UChar32,22> kCodePoints = {
        0x61, 0xdf, 0x901, 0x3040,
        0xac00, 0xd800, 0xdbff, 0xdcde,
        0xdffd, 0xe000, 0xffff, 0x10000,
        0x12345, 0xe0021, 0x10ffff, 0x110000,
        0x234567, 0x7fffffff, -1, -1000,
        0, 0x400
    };
    static const std::array<UChar, 21> kExpectedUnsafe = {
        0x61, 0xdf, 0x901, 0x3040,
        0xac00, 0xd800, 0xdbff, 0xdcde,
        0xdffd, 0xe000, 0xffff, 0xd800, 0xdc00,
        0xd808, 0xdf45, 0xdb40, 0xdc21, 0xdbff, 0xdfff, /* not 0x110000 */
        /* none from this line */
        0, 0x400
    };
  // clang-format on

  std::array<UChar, 100> buffer{};

  size_t actual_length = 0U;
  for (const UChar32 codepoint : kCodePoints) {
    if (codepoint < 0 || 0x10ffff < codepoint) {
      continue;    // skip non-code points for U16_APPEND_UNSAFE
    }

    U16AppendUnsafe(buffer.data(), actual_length, codepoint);
  }

  // the last appended location should be the same as the length of expected
  EXPECT_EQ(actual_length, kExpectedUnsafe.size());
  // expected buffer after appending
  EXPECT_TRUE(std::equal(
    buffer.begin(),
    std::next(
      buffer.begin(),
      static_cast<decltype(buffer)::difference_type>(actual_length)),
    kExpectedUnsafe.begin()));

  // The next part in TestAppend is not intended to test U16_APPEND_UNSAFE,
  // skip for now
}

// NOLINTEND(cppcoreguidelines-avoid-do-while, cppcoreguidelines-macro-usage)
}    // namespace longlp::base::icu
