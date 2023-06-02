// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

// Integrated from
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf8tst.c
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
  using internal::U8AppendUnsafe;
  using internal::U8Next;
}    // namespace

// Integrated from TestNextPrevChar() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf8tst.c
// Removed other macro functions since our API only supports U8_NEXT atm
TEST(UTF8Test, TestNextPrevChar) {
  // clang-format off
  static constexpr std::array<UChar32, 2> kUTF8ErrorValue = {
    0x15, 0x9f
  };
  static constexpr std::array<uint8_t,16> kInput = {
      0x61,
      0xf0, 0x90, 0x90, 0x81,
      0xc0, 0x80,  // non-shortest form
      0xf3, 0xbe,  // truncated
      0xc2,  // truncated
      0x61,
      0x81, 0x90, 0x90, 0xf0,  // "backwards" sequence
      0x00
  };
  static constexpr std::array<UChar32,64> kExpectedCodePoints = {
  //  next_safe_ns        next_safe_s          prev_safe_ns        prev_safe_s
      0x0061,             0x0061,              0x0000,             0x0000,
      0x10401,            0x10401,             kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  0x61,               0x61,
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[1], kUTF8ErrorValue[1],  kUTF8ErrorValue[1], kUTF8ErrorValue[1],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      0x61,               0x61,                kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  0x10401,            0x10401,
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  UTF_ERROR_VALUE,    UTF_ERROR_VALUE,
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[1], kUTF8ErrorValue[1],
      kUTF8ErrorValue[0], kUTF8ErrorValue[0],  kUTF8ErrorValue[0], kUTF8ErrorValue[0],
      0x0000,             0x0000,              0x0061,             0x0061
  };
  static constexpr std::array<size_t, 32> kMovedOffset = {
  //  next_safe    prev_safe_s
      1,           15,
      5,           14,
      3,           13,
      4,           12,
      5,           11,
      6,           10,
      7,           9,
      9,           7,
      9,           7,
      10,          6,
      11,          5,
      12,          1,
      13,          1,
      14,          1,
      15,          1,
      16,          0,
  };
  // clang-format on

  size_t i = 0U;
  size_t j = 0U;
  for (size_t offset = 0U; offset < kInput.size(); ++offset) {
    UChar32 actual_codepoint = 0;
    auto expected_codepoint  = kExpectedCodePoints.at(i);    // next_safe_ns
    auto actual_offset       = offset;

    U8Next(kInput.data(), actual_offset, kInput.size(), actual_codepoint);

    EXPECT_EQ(actual_offset, kMovedOffset.at(j)) << fmt::sprintf(
      "ERROR: U8Next failed to move the offset correctly at %d\n "
      "ExpectedOffset:%d Got %d\n",
      offset,
      kMovedOffset.at(j),
      actual_offset);

    if (UTF_IS_ERROR(expected_codepoint)) {
      expected_codepoint = U_SENTINEL;
    }

    EXPECT_EQ(actual_codepoint, expected_codepoint) << fmt::sprintf(
      "ERROR: U8Next failed at offset=%ld. Expected:%lx Got:%lx\n",
      offset,
      expected_codepoint,
      actual_codepoint);

    i += 4;
    j += 2;
  }
}

// Integrated from TestNulTerminated() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf8tst.c
// Removed other macro functions since our API only supports U8_NEXT atm
TEST(UTF8Test, TestNulTerminated) {
  // clang-format off
  static constexpr std::array<uint8_t, 23> kInput = {
        0x61, //  0
        0xf0, 0x90, 0x90, 0x81, //  1
        0xc0, //  5
        0x80, //  6
        0xdf, 0x80, //  7
        0xc2, //  9
        0x62, // 10
        0xfd, // 11
        0xbe, // 12
        0xe0, 0xa0, 0x80, // 13
        0xe2, 0x82, 0xac, // 16
        0xf0, 0x90, 0x90, // 19
        0x00 // 22
       // 23
  };
  static constexpr std::array<UChar32, 13> kResult = {
      0x61,
      0x10401,
      U_SENTINEL,  // C0 not a lead byte
      U_SENTINEL,  // 80
      0x7c0,
      U_SENTINEL,  // C2
      0x62,
      U_SENTINEL,  // FD not a lead byte
      U_SENTINEL,  // BE
      0x800,
      0x20ac,
      U_SENTINEL,  // truncated F0 90 90
      0
  };
  // clang-format on

  UChar32 actual_codepoint        = 0;
  size_t expected_codepoint_index = 0;
  size_t i                        = 0;
  do {
    const auto prev_i = i;
    U8Next(kInput.data(), i, -1, actual_codepoint);
    const auto expected_codepoint = kResult.at(expected_codepoint_index);
    EXPECT_EQ(actual_codepoint, expected_codepoint) << fmt::sprintf(
      "U8Next(from %d)=U+%04x != U+%04x\n",
      prev_i,
      actual_codepoint,
      expected_codepoint);
    ++expected_codepoint_index;
  }
  while (actual_codepoint != 0);

  // The next part in TestNulTerminated is not intended to test U8_NEXT, skip
  // for now
}

// Integrated from TestNextPrevNonCharacters() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf8tst.c
// Removed other macro functions since our API only supports U8_NEXT atm
TEST(UTF8Test, TestNextPrevNonCharacters) {
  // clang-format off
  // test non-characters
  static constexpr std::array<uint8_t, 18> kNonChars = {
      0xef, 0xb7, 0x90,       // U+fdd0
      0xef, 0xbf, 0xbf,       // U+feff
      0xf0, 0x9f, 0xbf, 0xbe, // U+1fffe
      0xf0, 0xbf, 0xbf, 0xbf, // U+3ffff
      0xf4, 0x8f, 0xbf, 0xbe  // U+10fffe
  };
  // clang-format on

  UChar32 actual_codepoint{};

  for (size_t idx = 0U; idx < kNonChars.size();) {
    U8Next(kNonChars.data(), idx, kNonChars.size(), actual_codepoint);
    // Keep U_IS_UNICODE_NONCHAR for testing purpose
    EXPECT_TRUE(U_IS_UNICODE_NONCHAR(actual_codepoint)) << fmt::sprintf(
      "U8_NEXT(before %d) failed to read a non-character\n",
      idx);
  }

  // The next part in TestNulTerminated is not intended to test U8_NEXT, skip
  // for now
}

// Integrated from TestAppend() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf8tst.c
// Removed other macro functions since our API only supports U8_APPEND atm
TEST(UTF8Test, TestAppend) {
  // clang-format off
  static constexpr std::array<UChar32, 22> kCodePoints = {
      0x61, 0xdf, 0x901, 0x3040,
      0xac00, 0xd800, 0xdbff, 0xdcde,
      0xdffd, 0xe000, 0xffff, 0x10000,
      0x12345, 0xe0021, 0x10ffff, 0x110000,
      0x234567, 0x7fffffff, -1, -1000,
      0, 0x400
  };
  static constexpr std::array<uint8_t,49> kExpectedUnsafe = {
      0x61,  0xc3, 0x9f,  0xe0, 0xa4, 0x81,  0xe3, 0x81, 0x80,
      0xea, 0xb0, 0x80,  0xed, 0xa0, 0x80,  0xed, 0xaf, 0xbf,  0xed, 0xb3,
      0x9e, 0xed, 0xbf, 0xbd,  0xee, 0x80, 0x80,  0xef, 0xbf, 0xbf,  0xf0,
      0x90, 0x80, 0x80, 0xf0, 0x92, 0x8d, 0x85,  0xf3, 0xa0, 0x80, 0xa1,
      0xf4, 0x8f, 0xbf, 0xbf,  // not 0x110000
      // none from this line
      0,  0xd0, 0x80
  };

  // clang-format on

  std::array<uint8_t, 100> buffer{};

  size_t appended_location = 0;
  for (UChar32 codepoint : kCodePoints) {
    if (codepoint < 0 || 0x10ffff < codepoint) {
      continue;    // skip non-code points for U8_APPEND_UNSAFE
    }

    U8AppendUnsafe(buffer.data(), appended_location, codepoint);
  }

  // the last appended location should be the same as the length of expected
  EXPECT_EQ(appended_location, kExpectedUnsafe.size());
  // expected buffer after appending
  EXPECT_TRUE(std::equal(
    buffer.begin(),
    std::next(
      buffer.begin(),
      static_cast<decltype(buffer)::difference_type>(appended_location)),
    kExpectedUnsafe.begin()));

  // The next part in TestAppend is not intended to test U8_APPEND_UNSAFE, skip
  // for now
}

// Integrated from TestSurrogates() in
// https://github.com/unicode-org/icu/blob/main/icu4c/source/test/cintltst/utf8tst.c
// Removed other macro functions since our API only supports U8_NEXT atm
TEST(UTF8Test, TestSurrogates) {
  // clang-format off
    static constexpr std::array<uint8_t, 18> b = {
        0xc3, 0x9f,             //  00DF
        0xed, 0x9f, 0xbf,       //  D7FF
        0xed, 0xa0, 0x81,       //  D801
        0xed, 0xbf, 0xbe,       //  DFFE
        0xee, 0x80, 0x80,       //  E000
        0xf0, 0x97, 0xbf, 0xbe  // 17FFE
    };
  // clang-format on

  // Keep U8_NEXT_UNSAFE, U_IS_SURROGATE for testing purpose
  for (size_t i = 0U; i < b.size();) {
    auto offset_unsafe       = i;
    UChar32 codepoint_unsafe = 0;
    U8_NEXT_UNSAFE(b, offset_unsafe, codepoint_unsafe);

    auto offset_safe       = i;
    UChar32 codepoint_safe = 0;
    U8Next(b.data(), offset_safe, b.size(), codepoint_safe);

    const auto is_surrogate = U_IS_SURROGATE(codepoint_unsafe);

    if (is_surrogate) {
      // U8_NEXT() returns <0 for surrogate code points
      EXPECT_LT(codepoint_safe, 0);
      // U8_NEXT() skips only the first byte of a surrogate byte sequence.
      EXPECT_EQ(offset_safe, i + 1);
    }
    else {
      // otherwise, safe and unsafe function should be the same
      EXPECT_EQ(codepoint_safe, codepoint_unsafe);
      EXPECT_EQ(offset_safe, offset_unsafe);
    }

    i = offset_unsafe;    // advance by one UTF-8 sequence
  }

  // The next part in TestSurrogates is not intended to test U8_NEXT, skip
  // for now
}

// NOLINTEND(cppcoreguidelines-avoid-do-while, cppcoreguidelines-macro-usage)
}    // namespace longlp::base::icu
