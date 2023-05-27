// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_TEST_TEST_UTILS_MOVE_ONLY_INT_H_
#define LONGLP_TEST_TEST_UTILS_MOVE_ONLY_INT_H_

#include <compare>

namespace longlp::base {

// A move-only class that holds an integer. This is designed for testing
// containers. See also MoveOnlyInt.
class MoveOnlyInt {
 public:
  explicit MoveOnlyInt(int data) :
    data_(data) {}

  MoveOnlyInt(MoveOnlyInt&& other) noexcept :
    data_(other.data_) {
    other.data_ = 0;
  }

  MoveOnlyInt(const MoveOnlyInt&)                    = delete;
  auto operator=(const MoveOnlyInt&) -> MoveOnlyInt& = delete;

  ~MoveOnlyInt() { data_ = 0; }

  auto operator=(MoveOnlyInt&& other) noexcept -> MoveOnlyInt& {
    data_       = other.data_;
    other.data_ = 0;
    return *this;
  }

  friend auto
  operator==(const MoveOnlyInt& lhs, const MoveOnlyInt& rhs) -> bool {
    return lhs.data_ == rhs.data_;
  }

  friend auto operator<=>(const MoveOnlyInt& lhs, const MoveOnlyInt& rhs) {
    return lhs.data_ <=> rhs.data_;
  }

  [[nodiscard]]
  auto data() const -> int {
    return data_;
  }

 private:
  volatile int data_;
};

}    // namespace longlp::base

#endif    // LONGLP_TEST_TEST_UTILS_MOVE_ONLY_INT_H_
