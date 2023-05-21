// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_NO_DESTRUCTOR_H_
#define LONGLP_INCLUDE_BASE_NO_DESTRUCTOR_H_

#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

#include "base/compiler_specific.h"

// NOLINTBEGIN(*-member-init)
namespace longlp::base {

namespace internal {
  template <typename T>
  concept NonTriviallyDestructible = !std::is_trivially_destructible_v<T>;

  // std::aligned_storage_t is deprecated in c++23
  template <NonTriviallyDestructible T>
  struct AlignedStorage {
    alignas(T) std::array<std::byte, sizeof(T)> _;
  };

}    // namespace internal

// Helper type to create a function-local static variable of type `T` when `T`
// has a non-trivial destructor. Storing a `T` in a `base::NoDestructor<T>` will
// prevent `~T()` from running, even when the variable goes out of scope.
//
// Useful when a variable has static storage duration but its type has a
// non-trivial destructor. Chromium bans global constructors and destructors:
// using a function-local static variable prevents the former, while using
// `base::NoDestructor<T>` prevents the latter.
//
// ## Caveats
//
// - Must only be used as a function-local static variable. Declaring a global
//   variable of type `base::NoDestructor<T>` will still generate a global
//   constructor; declaring a local or member variable will lead to memory leaks
//   or other surprising and undesirable behaviour.
//
// - If the data is rarely used, consider creating it on demand rather than
//   caching it for the lifetime of the program. Though `base::NoDestructor<T>`
//   does not heap allocate, the compiler still reserves space in bss for
//   storing `T`, which costs memory at runtime.
//
// - If `T` is trivially destructible, do not use `base::NoDestructor<T>`:
//
//     const uint64_t GetUnstableSessionSeed() {
//       // No need to use `base::NoDestructor<T>` as `uint64_t` is trivially
//       // destructible and does not require a global destructor.
//       static const uint64_t kSessionSeed = base::RandUint64();
//       return kSessionSeed;
//     }
//
// ## Example Usage
//
// const std::string& GetDefaultText() {
//   // Required since `static const std::string` requires a global destructor.
//   static const base::NoDestructor<std::string> s("Hello world!");
//   return *s;
// }
//
// More complex initialization using a lambda:
//
// const std::string& GetRandomNonce() {
//   // `nonce` is initialized with random data the first time this function is
//   // called, but its value is fixed thereafter.
//   static const base::NoDestructor<std::string> nonce([] {
//     std::string s(16);
//     crypto::RandString(s.data(), s.size());
//     return s;
//   }());
//   return *nonce;
// }
//
// ## Thread safety
//
// Initialisation of function-local static variables is thread-safe since C++11.
// The standard guarantees that:
//
// - function-local static variables will be initialised the first time
//   execution passes through the declaration.
//
// - if another thread's execution concurrently passes through the declaration
//   in the middle of initialisation, that thread will wait for the in-progress
//   initialisation to complete.
template <internal::NonTriviallyDestructible T>
requires requires(internal::AlignedStorage<T> storage) {
  // storage should large enough to hold the instance
  requires sizeof(storage) >= sizeof(T);
  // storage should meet the instance's alignment requirement
  requires alignof(decltype(storage)) >= alignof(T);
}
class NoDestructor {
 public:
  LONGLP_DIAGNOSTIC_PUSH
  // disable GCC complains about member initialization
  LONGLP_GCC_DIAGNOSTIC_IGNORED("-Weffc++")

  // Not constexpr; just write static constexpr T obj = ...; if the value should
  // be a constexpr.
  template <typename... Args>
  requires std::constructible_from<T, Args&&...>
  explicit NoDestructor(Args&&... args) {
    new (&instance_storage_) T(std::forward<Args>(args)...);
  }

  // Allows copy and move construction of the contained type, to allow
  // construction from an initializer list, e.g. for std::vector.
  explicit NoDestructor(const T& obj) { new (&instance_storage_) T(obj); }

  explicit NoDestructor(T&& obj) { new (&instance_storage_) T(std::move(obj)); }

  // Work-around for
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84849
  template <class U, class... Args>
  requires std::constructible_from<T, std::initializer_list<U>, Args&&...>
  constexpr NoDestructor(std::initializer_list<U> ilist, Args&&... args) {
    new (&instance_storage_) T(
      std::forward<std::initializer_list<U>>(ilist),
      std::forward<Args>(args)...);
  }

  LONGLP_DIAGNOSTIC_POP

  // no copy
  NoDestructor(const NoDestructor&)                    = delete;
  auto operator=(const NoDestructor&) -> NoDestructor& = delete;

  ~NoDestructor()                                      = default;

  auto operator*() const -> const T& { return *get(); }

  auto operator*() -> T& { return *get(); }

  auto operator->() const -> const T* { return get(); }

  auto operator->() -> T* { return get(); }

  auto get() const -> const T* {
    return std::bit_cast<const T*>(instance_storage_);
  }

  auto get() -> T* { return std::bit_cast<T*>(&instance_storage_); }

 private:
  // Use placement new to construct T in this pre-allocated memory.
  // For performance critical code, it should not be zero-initialized during
  // construction.
  internal::AlignedStorage<T> instance_storage_;

  // TODO(longlp, sanitizer): LSan
};

}    // namespace longlp::base

// NOLINTEND(*-member-init)

#endif    // LONGLP_INCLUDE_BASE_NO_DESTRUCTOR_H_
