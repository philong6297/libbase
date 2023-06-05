#include <base/strings/string_utils.h>

#include <array>
#include <vector>

#include <base/strings/typedefs.h>
#include <gtest/gtest.h>

#include "test_utils/gtest_fix_u8string_comparison.h"

namespace longlp::base {
// NOLINTBEGIN(*-avoid-c-arrays,hicpp-no-array-decay,cppcoreguidelines-pro-bounds-array-to-pointer-decay)
namespace {
  // Helper used to test TruncateUTF8ToByteSize.
  auto Truncated(
    const StringViewUTF8 input,
    const size_t byte_size,
    StringUTF8& output) -> bool {
    size_t prev = input.length();
    TruncateUTF8ToByteSize(input, byte_size, output);
    return prev != output.length();
  }
}    // namespace

TEST(StringUtilTest, TruncateUTF8ToByteSize) {
  StringUTF8 output;

  // Empty strings and invalid byte_size arguments
  EXPECT_FALSE(Truncated(StringUTF8(), 0, output));
  ExpectEQ(output, LONGLP_LITERAL_UTF8(""));
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xe1\x80\xbf"), 0, output));
  ExpectEQ(output, LONGLP_LITERAL_UTF8(""));
  EXPECT_FALSE(Truncated(
    LONGLP_LITERAL_UTF8("\xe1\x80\xbf"),
    static_cast<size_t>(-1),
    output));
  EXPECT_FALSE(Truncated(LONGLP_LITERAL_UTF8("\xe1\x80\xbf"), 4, output));

  // Testing the truncation of valid UTF8 correctly
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("abc"), 2, output));
  ExpectEQ(output, LONGLP_LITERAL_UTF8("ab"));
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xc2\x81\xc2\x81"), 2, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xc2\x81")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xc2\x81\xc2\x81"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xc2\x81")), 0);
  EXPECT_FALSE(Truncated(LONGLP_LITERAL_UTF8("\xc2\x81\xc2\x81"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xc2\x81\xc2\x81")), 0);

  {
    const CharUTF8 array[] = LONGLP_LITERAL_UTF8("\x00\x00\xc2\x81\xc2\x81");
    const StringUTF8 array_string(array, std::size(array));
    EXPECT_TRUE(Truncated(array_string, 4, output));
    EXPECT_EQ(
      output.compare(StringUTF8(LONGLP_LITERAL_UTF8("\x00\x00\xc2\x81"), 4)),
      0);
  }

  {
    const CharUTF8 array[] = LONGLP_LITERAL_UTF8("\x00\xc2\x81\xc2\x81");
    const StringUTF8 array_string(array, std::size(array));
    EXPECT_TRUE(Truncated(array_string, 4, output));
    EXPECT_EQ(
      output.compare(StringUTF8(LONGLP_LITERAL_UTF8("\x00\xc2\x81"), 3)),
      0);
  }

  // Testing invalid UTF8
  EXPECT_TRUE(
    Truncated(LONGLP_LITERAL_UTF8("\xed\xa0\x80\xed\xbf\xbf"), 6, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xed\xa0\x8f"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xed\xbf\xbf"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);

  // Testing invalid UTF8 mixed with valid UTF8
  EXPECT_FALSE(Truncated(LONGLP_LITERAL_UTF8("\xe1\x80\xbf"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xe1\x80\xbf")), 0);
  EXPECT_FALSE(Truncated(LONGLP_LITERAL_UTF8("\xf1\x80\xa0\xbf"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xf1\x80\xa0\xbf")), 0);
  EXPECT_FALSE(Truncated(
    LONGLP_LITERAL_UTF8("a\xc2\x81\xe1\x80\xbf\xf1\x80\xa0\xbf"),
    10,
    output));
  EXPECT_EQ(
    output.compare(LONGLP_LITERAL_UTF8(
      "a\xc2\x81\xe1\x80\xbf\xf1\x80\xa0"
      "\xbf")),
    0);
  EXPECT_TRUE(Truncated(
    LONGLP_LITERAL_UTF8(
      "a\xc2\x81\xe1\x80\xbf\xf1"
      "a"
      "\x80\xa0"),
    10,
    output));
  EXPECT_EQ(
    output.compare(LONGLP_LITERAL_UTF8(
      "a\xc2\x81\xe1\x80\xbf\xf1"
      "a")),
    0);
  EXPECT_FALSE(Truncated(
    LONGLP_LITERAL_UTF8(
      "\xef\xbb\xbf"
      "abc"),
    6,
    output));
  EXPECT_EQ(
    output.compare(LONGLP_LITERAL_UTF8(
      "\xef\xbb\xbf"
      "abc")),
    0);

  // Overlong sequences
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xc0\x80"), 2, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xc1\x80\xc1\x81"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xe0\x80\x80"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xe0\x82\x80"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xe0\x9f\xbf"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf0\x80\x80\x8D"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf0\x80\x82\x91"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf0\x80\xa0\x80"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf0\x8f\xbb\xbf"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(
    Truncated(LONGLP_LITERAL_UTF8("\xf8\x80\x80\x80\xbf"), 5, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(
    Truncated(LONGLP_LITERAL_UTF8("\xfc\x80\x80\x80\xa0\xa5"), 6, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);

  // Beyond U+10FFFF (the upper limit of Unicode codespace)
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf4\x90\x80\x80"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(
    Truncated(LONGLP_LITERAL_UTF8("\xf8\xa0\xbf\x80\xbf"), 5, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(
    Truncated(LONGLP_LITERAL_UTF8("\xfc\x9c\xbf\x80\xbf\x80"), 6, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);

  // BOMs in UTF-16(BE|LE) and UTF-32(BE|LE)
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xfe\xff"), 2, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xff\xfe"), 2, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);

  {
    const CharUTF8 array[] = LONGLP_LITERAL_UTF8("\x00\x00\xfe\xff");
    const StringUTF8 array_string(array, std::size(array));
    EXPECT_TRUE(Truncated(array_string, 4, output));
    EXPECT_EQ(
      output.compare(StringUTF8(LONGLP_LITERAL_UTF8("\x00\x00"), 2)),
      0);
  }

  // Variants on the previous test
  {
    const CharUTF8 array[] = LONGLP_LITERAL_UTF8("\xff\xfe\x00\x00");
    const StringUTF8 array_string(array, 4);
    EXPECT_FALSE(Truncated(array_string, 4, output));
    EXPECT_EQ(
      output.compare(StringUTF8(LONGLP_LITERAL_UTF8("\xff\xfe\x00\x00"), 4)),
      0);
  }
  {
    const CharUTF8 array[] = LONGLP_LITERAL_UTF8("\xff\x00\x00\xfe");
    const StringUTF8 array_string(array, std::size(array));
    EXPECT_TRUE(Truncated(array_string, 4, output));
    EXPECT_EQ(
      output.compare(StringUTF8(LONGLP_LITERAL_UTF8("\xff\x00\x00"), 3)),
      0);
  }

  // Non-characters : U+xxFFF[EF] where xx is 0x00 through 0x10 and <FDD0,FDEF>
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xef\xbf\xbe"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf0\x8f\xbf\xbe"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xf3\xbf\xbf\xbf"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xef\xb7\x90"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xef\xb7\xaf"), 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);

  // Strings in legacy encodings that are valid in UTF-8, but
  // are invalid as UTF-8 in real data.
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("caf\xe9"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("caf")), 0);
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xb0\xa1\xb0\xa2"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
  EXPECT_FALSE(Truncated(LONGLP_LITERAL_UTF8("\xa7\x41\xa6\x6e"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xa7\x41\xa6\x6e")), 0);
  EXPECT_TRUE(Truncated(
    LONGLP_LITERAL_UTF8("\xa7\x41\xa6\x6e\xd9\xee\xe4\xee"),
    7,
    output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xa7\x41\xa6\x6e")), 0);

  // Testing using the same string as input and output.
  EXPECT_FALSE(Truncated(output, 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xa7\x41\xa6\x6e")), 0);
  EXPECT_TRUE(Truncated(output, 3, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("\xa7\x41")), 0);

  // "abc" with U+201[CD] in windows-125[0-8]
  EXPECT_TRUE(Truncated(
    LONGLP_LITERAL_UTF8(
      "\x93"
      "abc\x94"),
    5,
    output));
  EXPECT_EQ(
    output.compare(LONGLP_LITERAL_UTF8(
      "\x93"
      "abc")),
    0);

  // U+0639 U+064E U+0644 U+064E in ISO-8859-6
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xd9\xee\xe4\xee"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);

  // U+03B3 U+03B5 U+03B9 U+03AC in ISO-8859-7
  EXPECT_TRUE(Truncated(LONGLP_LITERAL_UTF8("\xe3\xe5\xe9\xdC"), 4, output));
  EXPECT_EQ(output.compare(LONGLP_LITERAL_UTF8("")), 0);
}

// NOLINTEND(*-avoid-c-arrays,hicpp-no-array-decay,cppcoreguidelines-pro-bounds-array-to-pointer-decay)
}    // namespace longlp::base
