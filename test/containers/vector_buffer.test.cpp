// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/containers/vector_buffer.h>

#include <gtest/gtest.h>

#include "test_utils/copy_only_int.h"
#include "test_utils/move_only_int.h"

namespace longlp::base::internal {

namespace {
  constexpr auto kSize = 10U;
}    // namespace

// NOLINTBEGIN(*-narrowing-conversions,
// cppcoreguidelines-pro-bounds-pointer-arithmetic)
LONGLP_DIAGNOSTIC_PUSH
LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wsign-conversion")
LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wunsafe-buffer-usage")
LONGLP_GCC_DIAGNOSTIC_IGNORED("-Wsign-conversion")

TEST(VectorBuffer, DeletePOD) {
  VectorBuffer<int> buffer(kSize);
  for (auto i = 0U; i < kSize; ++i) {
    buffer[i] = i + 1;
  }

  VectorBuffer<int>::DestructRange(buffer.begin(), buffer.end());

  // Delete should do nothing.
  for (auto i = 0U; i < kSize; ++i) {
    EXPECT_EQ(i + 1, buffer[i]);
  }
}

TEST(VectorBuffer, DeleteMoveOnly) {
  VectorBuffer<MoveOnlyInt> buffer(kSize);
  for (auto i = 0U; i < kSize; ++i) {
    new (buffer.begin() + i) MoveOnlyInt(i + 1);
  }

  buffer.DestructRange(buffer.begin(), buffer.end());

  // Delete should have reset all of the values to 0.
  for (auto i = 0U; i < kSize; ++i) {
    EXPECT_EQ(0, buffer[i].data());
  }
}

TEST(VectorBuffer, PODMove) {
  VectorBuffer<int> dest(kSize);

  VectorBuffer<int> original(kSize);
  for (auto i = 0U; i < kSize; ++i) {
    original.begin()[i] = i + 1;
  }

  original.MoveRange(original.begin(), original.end(), dest.begin());
  for (auto i = 0U; i < kSize; ++i) {
    EXPECT_EQ(i + 1, dest.begin()[i]);
  }
}

TEST(VectorBuffer, MovableMove) {
  VectorBuffer<MoveOnlyInt> dest(kSize);

  VectorBuffer<MoveOnlyInt> original(kSize);
  for (auto i = 0U; i < kSize; ++i) {
    new (original.begin() + i) MoveOnlyInt(i + 1);
  }

  original.MoveRange(original.begin(), original.end(), dest.begin());

  // Moving from a MoveOnlyInt resets to 0.
  for (auto i = 0U; i < kSize; ++i) {
    EXPECT_EQ(0, original.begin()[i].data());
    EXPECT_EQ(i + 1, dest.begin()[i].data());
  }
}

TEST(VectorBuffer, CopyToMove) {
  VectorBuffer<CopyOnlyInt> dest(kSize);

  VectorBuffer<CopyOnlyInt> original(kSize);
  for (auto i = 0U; i < kSize; ++i) {
    new (original.begin() + i) CopyOnlyInt(i + 1);
  }

  original.MoveRange(original.begin(), original.end(), dest.begin());

  // The original should have been destructed, which should reset the value to
  // 0. Technically this dereferences the destructed object.
  for (auto i = 0U; i < kSize; ++i) {
    EXPECT_EQ(0, original.begin()[i].data());
    EXPECT_EQ(i + 1, dest.begin()[i].data());
  }
}

LONGLP_DIAGNOSTIC_POP
// NOLINTEND(*-narrowing-conversions,
// cppcoreguidelines-pro-bounds-pointer-arithmetic)

}    // namespace longlp::base::internal
