// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_TEST_TEST_UTILS_COPY_ONLY_INT_H_
#define LONGLP_TEST_TEST_UTILS_COPY_ONLY_INT_H_

#include <compare>

namespace longlp::base {

// A copy-only (not moveable) class that holds an integer. This is designed for
// testing containers. See also MoveOnlyInt.
class CopyOnlyInt {
 public:
  explicit CopyOnlyInt(int data) :
    data_(data) {}

  CopyOnlyInt(const CopyOnlyInt& other) :
    data_(other.data_) {
    ++num_copies_;
  }

  ~CopyOnlyInt() { data_ = 0; }

  CopyOnlyInt(CopyOnlyInt&&)                    = delete;
  auto operator=(CopyOnlyInt&) -> CopyOnlyInt&  = delete;
  auto operator=(CopyOnlyInt&&) -> CopyOnlyInt& = delete;

  friend auto
  operator==(const CopyOnlyInt& lhs, const CopyOnlyInt& rhs) -> bool {
    return lhs.data_ == rhs.data_;
  }

  friend auto operator<=>(const CopyOnlyInt& lhs, const CopyOnlyInt& rhs) {
    return lhs.data_ <=> rhs.data_;
  }

  [[nodiscard]]
  auto data() const -> int {
    return data_;
  }

  static void reset_num_copies() { num_copies_ = 0; }

  static auto num_copies() -> int { return num_copies_; }

 private:
  volatile int data_;

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static int num_copies_;
};

}    // namespace longlp::base

#endif    // LONGLP_TEST_TEST_UTILS_COPY_ONLY_INT_H_
