// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/auto_reset.h>

#include <gtest/gtest.h>

namespace longlp::base {
TEST(AutoResetTest, Constructing) {
  int value = 10;
  {
    AutoReset<int> reset_1{&value, 20};
    EXPECT_EQ(20, value);
    {
      value                  = 15;
      AutoReset<int> reset_2 = std::move(reset_1);
      // Moving to a new resetter does not change the value;
      EXPECT_EQ(15, value);
    }
    // Moved-to `reset_2` is out of scoped, and resets to the original value
    // that was in moved-from `reset_1`.
    EXPECT_EQ(10, value);
    value = 105;
  }
  // Moved-from `reset_1` does not reset to anything.
  EXPECT_EQ(105, value);
}
}    // namespace longlp::base
