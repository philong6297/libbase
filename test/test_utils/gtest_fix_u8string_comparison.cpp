// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "test_utils/gtest_fix_u8string_comparison.h"

#include <bit>
#include <string_view>

#include <gtest/gtest.h>

namespace longlp::base {
void ExpectEQ(StringViewUTF8 str_a, StringViewUTF8 str_b) {
  if (str_a == str_b) {
    SUCCEED();
    return;
  }
  // manually convert u8string to string
  ADD_FAILURE()
    << std::quoted(std::string_view(
         std::bit_cast<const CharASCII*>(str_a.data()),
         str_a.size()))
    << " vs "
    << std::quoted(std::string_view(
         std::bit_cast<const CharASCII*>(str_b.data()),
         str_b.size()));
}

auto ToStringASCII(StringViewUTF8 str) -> StringASCII {
  return {std::bit_cast<const CharASCII*>(str.data()), str.size()};
}
}    // namespace longlp::base
