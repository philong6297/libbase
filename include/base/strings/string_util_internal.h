// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_STRINGS_STRING_UTIL_INTERNAL_H_
#define LONGLP_INCLUDE_BASE_STRINGS_STRING_UTIL_INTERNAL_H_

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <string>
#include <string_view>

#include "base/compiler_specific.h"
#include "base/strings/typedefs.h"

namespace longlp::base::internal {
// ASCII-specific tolower.  The standard library's tolower is locale sensitive,
// so we don't want to use it here.
template <CharTraits CharT>
constexpr auto ToLowerASCII(CharT char_value) -> CharT {
  return (char_value >= 'A' && char_value <= 'Z')
         ? (char_value + ('a' - 'A'))
         : char_value;
}

// ASCII-specific toupper.  The standard library's toupper is locale sensitive,
// so we don't want to use it here.
template <CharTraits CharT>
constexpr auto ToUpperASCII(CharT char_value) -> CharT {
  return (char_value >= 'a' && char_value <= 'z')
         ? static_cast<CharT>(char_value + 'A' - 'a')
         : char_value;
}

// Converts the given string to its ASCII-lowercase equivalent. Non-ASCII
// bytes (or UTF-16 code units in `StringViewUTF16`) are permitted but will be
// unmodified.
template <CharTraits CharT>
auto ToLowerASCII(std::basic_string_view<CharT> str_view)
  -> std::basic_string<CharT> {
  std::basic_string<CharT> ret;
  ret.reserve(str_view.size());
  for (auto character : str_view) {
    ret.push_back(ToLowerASCII(character));
  }
  return ret;
}

// Converts the given string to its ASCII-uppercase equivalent. Non-ASCII
// bytes (or UTF-16 code units in `StringViewUTF16`) are permitted but will be
// unmodified.
template <CharTraits CharT>
auto ToUpperASCII(std::basic_string_view<CharT> str_view)
  -> std::basic_string<CharT> {
  std::basic_string<CharT> ret;
  ret.reserve(str_view.size());
  for (auto character : str_view) {
    ret.push_back(ToUpperASCII(character));
  }
  return ret;
}

// Like strcasecmp for ASCII case-insensitive comparisons only. Returns:
//   -1  (a < b)
//    0  (a == b)
//    1  (a > b)
// To compare all Unicode code points case-insensitively, use
// base::i18n::ToLower or base::i18n::FoldCase and then just call the normal
// string operators on the result.
//
// Non-ASCII bytes (or UTF-16 code units in `StringViewUTF16`) are permitted but
// will be compared unmodified.
template <CharTraits CharT>
constexpr auto CompareCaseInsensitiveASCII(
  std::basic_string_view<CharT> string_view_a,
  std::basic_string_view<CharT> string_view_b) -> int32_t {
  // Find the first characters that aren't equal and compare them.  If the end
  // of one of the strings is found before a nonequal character, the lengths
  // of the strings are compared. Compare using the unsigned type so the sort
  // order is independent of the signedness of `char`.
  using UCharT =
    std::make_unsigned_t<typename std::basic_string_view<CharT>::value_type>;
  for (
    size_t i = 0; i < string_view_a.length() && i < string_view_b.length();
    ++i) {
    const auto lower_a = static_cast<UCharT>(ToLowerASCII(string_view_a[i]));
    const auto lower_b = static_cast<UCharT>(ToLowerASCII(string_view_b[i]));
    if (lower_a < lower_b) {
      return -1;
    }
    if (lower_a > lower_b) {
      return 1;
    }
  }

  // End of one string hit before finding a different character. Expect the
  // common case to be "strings equal" at this point so check that first.
  if (string_view_a.length() == string_view_b.length()) {
    return 0;
  }

  if (string_view_a.length() < string_view_b.length()) {
    return -1;
  }
  return 1;
}

// Equality for ASCII case-insensitive comparisons. Non-ASCII bytes (or UTF-16
// code units in `StringViewUTF16`) are permitted but will be compared
// unmodified. To compare all Unicode code points case-insensitively, use
// base::i18n::ToLower or base::i18n::FoldCase and then compare with either ==
// or !=.
template <CharTraits CharT, CharTraits CharU>
constexpr auto EqualsCaseInsensitiveASCII(
  std::basic_string_view<CharT> string_view_a,
  std::basic_string_view<CharU> string_view_b) -> bool {
  return std::ranges::
    equal(string_view_a, string_view_b, [](CharT char_a, CharU char_b) {
      // Intended signess compare
      // NOLINTBEGIN(bugprone-signed-char-misuse)
      LONGLP_DIAGNOSTIC_PUSH
      LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wsign-compare")
      return internal::ToLowerASCII(char_a) == internal::ToLowerASCII(char_b);
      LONGLP_DIAGNOSTIC_POP
      // NOLINTEND(bugprone-signed-char-misuse)
    });
}

// A Matcher for DoReplaceMatchesAfterOffset() that matches substrings.
template <CharTraits CharT>
struct SubstringMatcher {
  std::basic_string_view<CharT> find_this;

  constexpr auto
  Find(const std::basic_string_view<CharT> input, const size_t pos) -> size_t {
    return input.find(find_this.data(), pos, find_this.length());
  }

  constexpr auto MatchSize() -> size_t { return find_this.length(); }
};

// A Matcher for DoReplaceMatchesAfterOffset() that matches single characters.
template <CharTraits CharT>
struct CharacterMatcher {
  std::basic_string_view<CharT> find_any_of_these;

  constexpr auto
  Find(const std::basic_string_view<CharT> input, const size_t pos) -> size_t {
    return input
      .find_first_of(find_any_of_these.data(), pos, find_any_of_these.length());
  }

  constexpr auto MatchSize() -> size_t { return 1; }
};

enum class ReplaceType {
  kReplaceAll,
  kReplaceFirst
};

// Runs in O(n) time in the length of |str|, and transforms the string without
// reallocating when possible. Returns |true| if any matches were found.
//
// This is parameterized on a |Matcher| traits type, so that it can be the
// implementation for both ReplaceChars() and ReplaceSubstringsAfterOffset().
template <typename Matcher, CharTraits CharT>
requires requires(
  std::basic_string_view<CharT> input,
  size_t pos,
  Matcher matcher) {
  { matcher.Find(input, pos) } -> std::same_as<size_t>;
  { matcher.MatchSize() } -> std::same_as<size_t>;
}
auto DoReplaceMatchesAfterOffset(
  std::basic_string<CharT>& str,
  size_t initial_offset,
  Matcher matcher,
  std::basic_string_view<CharT> replace_with,
  ReplaceType replace_type) -> bool {
  LONGLP_DIAGNOSTIC_PUSH
  LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wunsafe-buffer-usage")
  using Traits             = std::char_traits<CharT>;

  const size_t find_length = matcher.MatchSize();
  if (!find_length) {
    return false;
  }

  // If the find string doesn't appear, there's nothing to do.
  const size_t first_match = matcher.Find(str, initial_offset);
  if (first_match == std::basic_string_view<CharT>::npos) {
    return false;
  }

  // If we're only replacing one instance, there's no need to do anything
  // complicated.
  const size_t replace_length = replace_with.length();
  if (replace_type == ReplaceType::kReplaceFirst) {
    str.replace(first_match, find_length, replace_with.data(), replace_length);
    return true;
  }

  // If the find and replace strings are the same length, we can simply use
  // replace() on each instance, and finish the entire operation in O(n) time.
  if (find_length == replace_length) {
    auto* buffer = str.data();
    for (
      size_t offset = first_match; offset != std::basic_string<CharT>::npos;
      offset        = matcher.Find(str, offset + replace_length)) {
      Traits::copy(buffer + offset, replace_with.data(), replace_length);
    }
    return true;
  }

  // Since the find and replace strings aren't the same length, a loop like the
  // one above would be O(n^2) in the worst case, as replace() will shift the
  // entire remaining string each time. We need to be more clever to keep things
  // O(n).
  //
  // When the string is being shortened, it's possible to just shift the matches
  // down in one pass while finding, and truncate the length at the end of the
  // search.
  //
  // If the string is being lengthened, more work is required. The strategy used
  // here is to make two find() passes through the string. The first pass counts
  // the number of matches to determine the new size. The second pass will
  // either construct the new string into a new buffer (if the existing buffer
  // lacked capacity), or else -- if there is room -- create a region of scratch
  // space after |first_match| by shifting the tail of the string to a higher
  // index, and doing in-place moves from the tail to lower indices thereafter.
  size_t str_length = str.length();
  size_t expansion  = 0;
  if (replace_length > find_length) {
    // This operation lengthens the string; determine the new length by counting
    // matches.
    const size_t expansion_per_match = (replace_length - find_length);
    size_t num_matches               = 0;
    for (
      size_t match = first_match; match != std::basic_string<CharT>::npos;
      match        = matcher.Find(str, match + find_length)) {
      expansion += expansion_per_match;
      ++num_matches;
    }
    const size_t final_length = str_length + expansion;

    if (str.capacity() < final_length) {
      // If we'd have to allocate a new buffer to grow the string, build the
      // result directly into the new allocation via append().
      std::basic_string<CharT> src(str.get_allocator());
      str.swap(src);
      str.reserve(final_length);

      size_t pos = 0;
      for (size_t match = first_match;; match = matcher.Find(src, pos)) {
        str.append(src, pos, match - pos);
        str.append(replace_with.data(), replace_length);
        pos = match + find_length;

        // A mid-loop test/break enables skipping the final Find() call; the
        // number of matches is known, so don't search past the last one.
        if (!--num_matches) {
          break;
        }
      }

      // Handle substring after the final match.
      str.append(src, pos, str_length - pos);
      return true;
    }

    // Prepare for the copy/move loop below -- expand the string to its final
    // size by shifting the data after the first match to the end of the resized
    // string.
    size_t shift_src = first_match + find_length;
    size_t shift_dst = shift_src + expansion;

    // Big |expansion| factors (relative to |str_length|) require padding up to
    // |shift_dst|.
    if (shift_dst > str_length) {
      str.resize(shift_dst);
    }

    str.replace(
      shift_dst,
      str_length - shift_src,
      str,
      shift_src,
      str_length - shift_src);
    str_length = final_length;
  }

  // We can alternate replacement and move operations. This won't overwrite the
  // unsearched region of the string so long as |write_offset| <= |read_offset|;
  // that condition is always satisfied because:
  //
  //   (a) If the string is being shortened, |expansion| is zero and
  //       |write_offset| grows slower than |read_offset|.
  //
  //   (b) If the string is being lengthened, |write_offset| grows faster than
  //       |read_offset|, but |expansion| is big enough so that |write_offset|
  //       will only catch up to |read_offset| at the point of the last match.
  auto* buffer        = str.data();
  size_t write_offset = first_match;
  size_t read_offset  = first_match + expansion;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-do-while)
  do {
    if (replace_length) {
      Traits::copy(buffer + write_offset, replace_with.data(), replace_length);
      write_offset += replace_length;
    }
    read_offset += find_length;

    // min() clamps std::basic_string<CharT>::npos (the largest unsigned value)
    // to str_length.
    size_t match  = std::min(matcher.Find(str, read_offset), str_length);

    size_t length = match - read_offset;
    if (length) {
      Traits::move(buffer + write_offset, buffer + read_offset, length);
      write_offset += length;
      read_offset += length;
    }
  }
  while (read_offset < str_length);

  // If we're shortening the string, truncate it now.
  str.resize(write_offset);
  return true;
  LONGLP_DIAGNOSTIC_POP
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
template <CharTraits CharT>
auto ReplaceChars(
  std::basic_string_view<CharT> input,
  std::basic_string_view<CharT> find_any_of_these,
  std::basic_string_view<CharT> replace_with,
  std::basic_string<CharT>& output) -> bool {
  // Commonly, this is called with output and input being the same string; in
  // that case, skip the copy.
  if (input.data() != output.data() || input.size() != output.size()) {
    output.assign(input.data(), input.size());
  }

  return DoReplaceMatchesAfterOffset(
    output,
    0,
    CharacterMatcher<CharT>{find_any_of_these},
    replace_with,
    ReplaceType::kReplaceAll);
}

// NOLINTEND(bugprone-easily-swappable-parameters)

}    // namespace longlp::base::internal

#endif    // LONGLP_INCLUDE_BASE_STRINGS_STRING_UTIL_INTERNAL_H_
