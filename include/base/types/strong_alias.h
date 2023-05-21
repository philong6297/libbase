// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_TYPES_STRONG_ALIAS_H_
#define LONGLP_INCLUDE_BASE_TYPES_STRONG_ALIAS_H_

#include <concepts>
#include <functional>
#include <ostream>
#include <type_traits>

#include "base/compiler_specific.h"

namespace longlp::base {

namespace internal {
  // the above #include of <ostream> is necessary to guarantee consistent
  // results here for basic types.
  template <typename T>
  concept SupportsOstreamOperator = requires(std::ostream& ostream, T value) {
    ostream << value;
  };

}    // namespace internal

// A type-safe alternative for a typedef or a 'using' directive.
//
// C++ currently does not support type-safe typedefs, despite multiple proposals
// (ex. http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3515.pdf). The
// next best thing is to try and emulate them in library code.
//
// The motivation is to disallow several classes of errors:
//
// using Orange = int;
// using Apple = int;
// Apple apple(2);
// Orange orange = apple;  // Orange should not be able to become an Apple.
// Orange x = orange + apple;  // Shouldn't add Oranges and Apples.
// if (orange > apple);  // Shouldn't compare Apples to Oranges.
// void foo(Orange);
// void foo(Apple);  // Redefinition.
// etc.
//
// StrongAlias may instead be used as follows:
//
// using Orange = StrongAlias<class OrangeTag, int>;
// using Apple = StrongAlias<class AppleTag, int>;
// using Banana = StrongAlias<class BananaTag, std::string>;
// Apple apple(2);
// Banana banana("Hello");
// Orange orange = apple;  // Does not compile.
// Orange other_orange = orange;  // Compiles, types match.
// Orange x = orange + apple;  // Does not compile.
// Orange y = Orange(orange.value() + apple.value());  // Compiles.
// Orange z = Orange(banana->size() + *other_orange);  // Compiles.
// if (orange > apple);  // Does not compile.
// if (orange > other_orange);  // Compiles.
// void foo(Orange);
// void foo(Apple);  // Compiles into separate overload.
//
// StrongAlias is a zero-cost abstraction, it's compiled away.
//
// TagType is an empty tag class (also called "phantom type") that only serves
// the type system to differentiate between different instantiations of the
// template.
// UnderlyingType may be almost any value type. Note that some methods of the
// StrongAlias may be unavailable (ie. produce elaborate compilation errors when
// used) if UnderlyingType doesn't support them.
//
// StrongAlias only directly exposes comparison operators (for convenient use in
// ordered containers) and a Hasher struct (for unordered_map/set). It's
// impossible, without reflection, to expose all methods of the UnderlyingType
// in StrongAlias's interface. It's also potentially unwanted (ex. you don't
// want to be able to add two StrongAliases that represent socket handles).
// A getter and dereference operators are provided in case you need to access
// the UnderlyingType.
//
// See also
// -
// https://chromium.googlesource.com/chromium/src/+/main/styleguide/c++/blink-c++.md#Prefer-enums-or-StrongAliases-to-bare-bools-for-function-parameters
// which provides recommendation and examples of
//   using StrongAlias<Tag, bool> instead of a bare bool.
// - IdType<...> which provides helpers for specializing StrongAlias to be
//   used as an id.
template <typename TagType, typename UnderlyingType>
class StrongAlias {
 public:
  // the default inilization behavior of StrongAlias should be the same as
  // UnderlyingType so we should not member-init in default-constructor
  LONGLP_DIAGNOSTIC_PUSH
  // disable GCC complains about member initialization
  LONGLP_GCC_DIAGNOSTIC_IGNORED("-Weffc++")
  constexpr StrongAlias() = default;
  LONGLP_DIAGNOSTIC_POP

  constexpr explicit StrongAlias(const UnderlyingType& value) :
    value_(value) {}

  constexpr explicit StrongAlias(UnderlyingType&& value) noexcept :
    value_(std::move(value)) {}

  //
  // Accesses the contained value by reference/pointer
  //

  constexpr auto operator->() -> UnderlyingType* { return &value_; }

  constexpr auto operator->() const -> const UnderlyingType* { return &value_; }

  constexpr auto operator*() & -> UnderlyingType& { return value_; }

  constexpr auto operator*() const& -> const UnderlyingType& { return value_; }

  constexpr auto operator*() && -> UnderlyingType&& {
    return std::move(value_);
  }

  constexpr auto operator*() const&& -> const UnderlyingType&& {
    return std::move(value_);
  }

  //
  // returns the contained value
  //
  constexpr auto value() & -> UnderlyingType& { return value_; }

  constexpr auto value() const& -> const UnderlyingType& { return value_; }

  constexpr auto value() && -> UnderlyingType&& { return std::move(value_); }

  constexpr auto value() const&& -> const UnderlyingType&& {
    return std::move(value_);
  }

  // explicit cast to underlying type
  constexpr explicit operator const UnderlyingType&() const& { return value_; }

  // expose equality comparision
  constexpr auto operator==(const StrongAlias& other) const -> bool {
    return value_ == other.value_;
  }

  // expose ordering comparision
  constexpr auto operator<=>(const StrongAlias& other) const {
    return value_ <=> other.value_;
  }

  // Hasher to use in std::unordered_map, std::unordered_set, etc.
  //
  // Example usage:
  //     using MyType = base::StrongAlias<...>;
  //     using MySet = std::unordered_set<MyType, typename MyType::Hasher>;
  //
  // https://google.github.io/styleguide/cppguide.html#std_hash asks to avoid
  // defining specializations of `std::hash` - this is why the hasher needs to
  // be explicitly specified and why the following code will *not* work:
  //     using MyType = base::StrongAlias<...>;
  //     using MySet = std::unordered_set<MyType>;  // This won't work.
  struct Hasher {
    using argument_type = StrongAlias;
    using result_type   = std::size_t;

    auto operator()(const argument_type& alias) const -> result_type {
      return std::hash<UnderlyingType>()(alias.value());
    }
  };

 private:
  UnderlyingType value_;
};

// Stream operator for convenience, streams the UnderlyingType.
template <typename TagType, internal::SupportsOstreamOperator UnderlyingType>
auto operator<<(
  std::ostream& stream,
  const StrongAlias<TagType, UnderlyingType>& alias) -> std::ostream& {
  return stream << alias.value();
}

}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_TYPES_STRONG_ALIAS_H_
