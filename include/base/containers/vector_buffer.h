// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_CONTAINERS_VECTOR_BUFFER_H_
#define LONGLP_INCLUDE_BASE_CONTAINERS_VECTOR_BUFFER_H_

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>

#include "base/assert.h"
#include "base/compiler_specific.h"

namespace longlp::base::internal {

// NOLINTBEGIN(*-no-malloc, cppcoreguidelines-pro-bounds-pointer-arithmetic)

LONGLP_DIAGNOSTIC_PUSH
LONGLP_CLANG_DIAGNOSTIC_IGNORED("-Wunsafe-buffer-usage")

// Internal implementation detail of base/containers.
//
// Implements a vector-like buffer that holds a certain capacity of T. Unlike
// std::vector, VectorBuffer never constructs or destructs its arguments, and
// can't change sizes. But it does implement templates to assist in efficient
// moving and destruction of those items manually.
//
// In particular, the destructor function does not iterate over the items if
// there is no destructor. Moves should be implemented as a memcpy/memmove for
// trivially copyable objects (POD) otherwise, it should be a std::move if
// possible, and as a last resort it falls back to a copy. This behavior is
// similar to std::vector.
//
// No special consideration is done for noexcept move constructors since
// we compile without exceptions.
//
// The current API does not support moving overlapping ranges.
template <typename T>
class VectorBuffer {
 public:
  constexpr VectorBuffer() = default;

  // TODO(longlp, sanitizer): Clang CFI
  explicit VectorBuffer(size_t count) :
    buffer_(std::bit_cast<T*>(malloc(sizeof(T) * count))),
    capacity_(count) {}

  VectorBuffer(VectorBuffer&& other) noexcept :
    buffer_(other.buffer_),
    capacity_(other.capacity_) {
    other.buffer_   = nullptr;
    other.capacity_ = 0;
  }

  VectorBuffer(const VectorBuffer&)                    = delete;
  auto operator=(const VectorBuffer&) -> VectorBuffer& = delete;

  ~VectorBuffer() { free(buffer_); }

  auto operator=(VectorBuffer&& other) noexcept -> VectorBuffer& {
    free(buffer_);
    buffer_         = other.buffer_;
    capacity_       = other.capacity_;

    other.buffer_   = nullptr;
    other.capacity_ = 0;
    return *this;
  }

  [[nodiscard]]
  auto capacity() const -> size_t {
    return capacity_;
  }

  // unsafe access
  auto operator[](size_t index) -> T& { return buffer_[index]; }

  auto operator[](size_t index) const -> const T& { return buffer_[index]; }

  auto begin() -> T* { return buffer_; }

  auto end() -> T* { return &buffer_[capacity_]; }

  // DestructRange
  // ------------------------------------------------------------

  static void DestructRange(T* begin, T* end) {
    // Non-trivially destructible objects must have their destructors called
    // individually.
    // Trivially destructible objects need not have their destructors called.
    if constexpr (!std::is_trivially_destructible_v<T>) {
      while (begin != end) {
        begin->~T();
        begin++;
      }
    }
  }

  // MoveRange
  // ----------------------------------------------------------------
  //
  // The destructor will be called (as necessary) for all moved types. The
  // ranges must not overlap.
  //
  // The parameters and begin and end (one past the last) of the input buffer,
  // and the address of the first element to copy to. There must be sufficient
  // room in the destination for all items in the range [begin, end).
  static void MoveRange(T* from_begin, T* from_end, T* to) {
    LONGLP_EXPECTS(!RangesOverlap(from_begin, from_end, to));

    // Trivially copyable types can use memcpy. trivially copyable implies
    // that there is a trivial destructor as we don't have to call it.
    if constexpr (std::is_trivially_copyable_v<T>) {
      memcpy(
        to,
        from_begin,
        static_cast<size_t>((from_end - from_begin)) * sizeof(T));
    }
    // Not trivially copyable, but movable: call the move constructor and
    // destruct the original.
    else if constexpr (std::is_move_constructible_v<T>) {
      while (from_begin != from_end) {
        new (to) T(std::move(*from_begin));
        from_begin->~T();
        from_begin++;
        to++;
      }
    }
    // Not movable, not trivially copyable: call the copy constructor and
    // destruct the original.
    else {
      while (from_begin != from_end) {
        new (to) T(*from_begin);
        from_begin->~T();
        from_begin++;
        to++;
      }
    }
  }

 private:
  static auto
  RangesOverlap(const T* from_begin, const T* from_end, const T* to) -> bool {
    return !(to >= from_end || to + (from_end - from_begin) <= from_begin);
  }

  T* buffer_       = nullptr;
  size_t capacity_ = 0U;
};

LONGLP_DIAGNOSTIC_POP

// NOLINTEND(*-no-malloc, cppcoreguidelines-pro-bounds-pointer-arithmetic)

}    // namespace longlp::base::internal

#endif    // LONGLP_INCLUDE_BASE_CONTAINERS_VECTOR_BUFFER_H_
