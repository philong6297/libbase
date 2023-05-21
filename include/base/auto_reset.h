// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_LIBBASE_AUTO_RESET_H_
#define LONGLP_INCLUDE_LIBBASE_AUTO_RESET_H_

#include <concepts>
#include <utility>

// base::AutoReset<> is useful for setting a variable to a new value only within
// a particular scope. An base::AutoReset<> object resets a variable to its
// original value upon destruction, making it an alternative to writing
// "var = false;" or "var = old_val;" at all of a block's exit points.
//
// This should be obvious, but note that an base::AutoReset<> instance should
// have a shorter lifetime than its scoped_variable, to prevent invalid memory
// writes when the base::AutoReset<> object is destroyed.

namespace longlp::base {

template <typename T>
class [[maybe_unused, nodiscard]] AutoReset {
 public:
  template <typename U>
  requires std::convertible_to<T, U>
  AutoReset(T* scoped_variable, U&& new_value) :
    scoped_variable_(scoped_variable),
    original_value_(
      std::exchange(*scoped_variable_, std::forward<U>(new_value))) {}

  AutoReset(AutoReset&& other) noexcept :
    scoped_variable_(std::exchange(other.scoped_variable_, nullptr)),
    original_value_(std::move(other.original_value_)) {}

  auto operator=(AutoReset&& rhs) noexcept -> AutoReset& {
    scoped_variable_ = std::exchange(rhs.scoped_variable_, nullptr);
    original_value_  = std::move(rhs.original_value_);
    return *this;
  }

  AutoReset(const AutoReset&)                    = default;
  auto operator=(const AutoReset&) -> AutoReset& = default;

  ~AutoReset() {
    if (scoped_variable_ != nullptr) {
      *scoped_variable_ = std::move(original_value_);
    }
  }

 private:
  T* scoped_variable_;
  T original_value_;
};

}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_LIBBASE_AUTO_RESET_H_
