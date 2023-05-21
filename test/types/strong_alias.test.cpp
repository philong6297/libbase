// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/types/strong_alias.h>

#include <concepts>
#include <cstdint>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <base/assert.h>
#include <gtest/gtest.h>

// NOLINTBEGIN(misc-redundant-expression)

namespace longlp::base {

namespace {

  // For test correctnenss, it's important that these getters return lexically
  // incrementing values as |index| grows.
  template <typename T>
  auto ConstructValue(const int32_t base_value) -> T {
    if constexpr (std::same_as<int, T>) {
      return 5 + base_value;
    }

    LONGLP_EXPECTS(base_value >= 0);

    if constexpr (std::same_as<uint64_t, T>) {
      return 500U + static_cast<uint64_t>(base_value);
    }

    if constexpr (std::same_as<std::string, T>) {
      return std::string(static_cast<size_t>(base_value), 'a');
    }
  }

  auto StreamOutputSame(const auto& a, const auto& b) -> bool {
    std::stringstream ssa;
    ssa << a;
    std::stringstream ssb;
    ssb << b;
    return ssa.str() == ssb.str();
  }

}    // namespace

template <typename T>
class StrongAliasTest : public ::testing::Test {};

using TestedTypes = ::testing::Types<int, uint64_t, std::string>;
TYPED_TEST_SUITE(StrongAliasTest, TestedTypes);

// This next test is compile-time, and thus not in an actual TEST since it
// would just result in running an empty test.
TYPED_TEST(StrongAliasTest, StreamOperatorExists) {
  // Aliases of ints should be streamable because ints are streamable.
  using StreamableAlias = StrongAlias<class IntTag, TypeParam>;
  static_assert(internal::SupportsOstreamOperator<StreamableAlias>);

  // Aliases of a class which does not expose a stream operator should
  // themselves not be streamable.
  struct Scope {};
  using NonStreamableAlias = StrongAlias<class ScopeTag, Scope>;
  static_assert(!internal::SupportsOstreamOperator<NonStreamableAlias>);
}

TYPED_TEST(StrongAliasTest, ValueAccessesUnderlyingValue) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;

  // Const value getter.
  const FooAlias const_alias(ConstructValue<TypeParam>(1));
  EXPECT_EQ(ConstructValue<TypeParam>(1), const_alias.value());
  static_assert(
    std::is_const_v<std::remove_reference_t<decltype(const_alias.value())>>,
    "Reference returned by const value getter should be const.");
}

TYPED_TEST(StrongAliasTest, ExplicitConversionToUnderlyingValue) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;

  const FooAlias const_alias(ConstructValue<TypeParam>(1));
  EXPECT_EQ(ConstructValue<TypeParam>(1), static_cast<TypeParam>(const_alias));
}

TYPED_TEST(StrongAliasTest, CanBeCopyConstructed) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  FooAlias alias(ConstructValue<TypeParam>(0));
  FooAlias copy_constructed = alias;
  EXPECT_EQ(copy_constructed, alias);

  FooAlias copy_assigned;
  copy_assigned = alias;
  EXPECT_EQ(copy_assigned, alias);
}

TYPED_TEST(StrongAliasTest, CanBeMoveConstructed) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  FooAlias alias(ConstructValue<TypeParam>(0));
  FooAlias move_constructed = std::move(alias);
  EXPECT_EQ(move_constructed, FooAlias(ConstructValue<TypeParam>(0)));

  FooAlias alias2(ConstructValue<TypeParam>(2));
  FooAlias move_assigned;
  move_assigned = std::move(alias2);
  EXPECT_EQ(move_assigned, FooAlias(ConstructValue<TypeParam>(2)));

  // Check that FooAlias is nothrow move constructible. This matters for
  // performance when used in std::vectors.
  static_assert(
    std::is_nothrow_move_constructible_v<FooAlias>,
    "Error: Alias is not nothow move constructible");
}

TYPED_TEST(StrongAliasTest, CanBeConstructedFromMoveOnlyType) {
  // Note, using a move-only unique_ptr to T:
  using FooAlias = StrongAlias<class FooTag, std::unique_ptr<TypeParam>>;

  FooAlias a(std::make_unique<TypeParam>(ConstructValue<TypeParam>(0)));
  EXPECT_EQ(*a.value(), ConstructValue<TypeParam>(0));

  auto bare_value = std::make_unique<TypeParam>(ConstructValue<TypeParam>(1));
  FooAlias b(std::move(bare_value));
  EXPECT_EQ(*b.value(), ConstructValue<TypeParam>(1));
}

TYPED_TEST(StrongAliasTest, MutableOperatorArrow) {
  // Note, using a move-only unique_ptr to T:
  using Ptr      = std::unique_ptr<TypeParam>;
  using FooAlias = StrongAlias<class FooTag, Ptr>;

  FooAlias a(std::make_unique<TypeParam>());
  EXPECT_TRUE(a.value());

  // Check that `a` can be modified through the use of operator->.
  a->reset();

  EXPECT_FALSE(a.value());
}

TYPED_TEST(StrongAliasTest, MutableOperatorStar) {
  // Note, using a move-only unique_ptr to T:
  using Ptr      = std::unique_ptr<TypeParam>;
  using FooAlias = StrongAlias<class FooTag, Ptr>;

  FooAlias a(std::make_unique<TypeParam>());
  FooAlias b(std::make_unique<TypeParam>());
  EXPECT_TRUE(*a);
  EXPECT_TRUE(*b);

  // Check that both the mutable l-value and r-value overloads work and we can
  // move out of the aliases.
  { Ptr ignore(*std::move(a)); }
  { Ptr ignore(std::move(*b)); }

  EXPECT_FALSE(a.value());
  EXPECT_FALSE(b.value());
}

TYPED_TEST(StrongAliasTest, MutableValue) {
  // Note, using a move-only unique_ptr to T:
  using Ptr      = std::unique_ptr<TypeParam>;
  using FooAlias = StrongAlias<class FooTag, Ptr>;

  FooAlias a(std::make_unique<TypeParam>());
  FooAlias b(std::make_unique<TypeParam>());
  EXPECT_TRUE(a.value());
  EXPECT_TRUE(b.value());

  // Check that both the mutable l-value and r-value overloads work and we can
  // move out of the aliases.
  { Ptr ignore(std::move(a).value()); }
  { Ptr ignore(std::move(b.value())); }

  EXPECT_FALSE(a.value());
  EXPECT_FALSE(b.value());
}

TYPED_TEST(StrongAliasTest, CanBeWrittenToOutputStream) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;

  const FooAlias a(ConstructValue<TypeParam>(0));
  EXPECT_TRUE(StreamOutputSame(ConstructValue<TypeParam>(0), a)) << a;
}

TYPED_TEST(StrongAliasTest, SizeSameAsUnderlyingType) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  static_assert(
    sizeof(FooAlias) == sizeof(TypeParam),
    "StrongAlias should be as large as the underlying type.");
}

TYPED_TEST(StrongAliasTest, IsDefaultConstructible) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  static_assert(
    std::is_default_constructible_v<FooAlias>,
    "Should be possible to default-construct a StrongAlias.");
  static_assert(
    std::is_trivially_default_constructible_v<FooAlias> ==
      std::is_trivially_default_constructible_v<TypeParam>,
    "Should be possible to trivially default-construct a StrongAlias iff the "
    "underlying type is trivially default constructible.");
}

TEST(StrongAliasTest, TrivialTypeAliasIsStandardLayout) {
  using FooAlias = StrongAlias<class FooTag, int>;
  static_assert(
    std::is_standard_layout_v<FooAlias>,
    "int-based alias should have standard layout. ");
  static_assert(
    std::is_trivially_copyable_v<FooAlias>,
    "int-based alias should be trivially copyable. ");
}

TYPED_TEST(StrongAliasTest, CannotBeCreatedFromDifferentAlias) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  using BarAlias = StrongAlias<class BarTag, TypeParam>;
  static_assert(
    !std::is_constructible_v<FooAlias, BarAlias>,
    "Should be impossible to construct FooAlias from a BarAlias.");
  static_assert(
    !std::is_convertible_v<BarAlias, FooAlias>,
    "Should be impossible to convert a BarAlias into FooAlias.");
}

TYPED_TEST(StrongAliasTest, CannotBeImplicitlyConverterToUnderlyingValue) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  static_assert(
    !std::is_convertible_v<FooAlias, TypeParam>,
    "Should be impossible to implicitly convert a StrongAlias into "
    "an underlying type.");
}

TYPED_TEST(StrongAliasTest, ComparesEqualToSameValue) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  // Comparison to self:
  const FooAlias a = FooAlias(ConstructValue<TypeParam>(0));
  EXPECT_EQ(a, a);
  EXPECT_FALSE(a != a);
  EXPECT_TRUE(a >= a);
  EXPECT_TRUE(a <= a);
  EXPECT_FALSE(a > a);
  EXPECT_FALSE(a < a);
  // Comparison to other equal object:
  const FooAlias b = FooAlias(ConstructValue<TypeParam>(0));
  EXPECT_EQ(a, b);
  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a >= b);
  EXPECT_TRUE(a <= b);
  EXPECT_FALSE(a > b);
  EXPECT_FALSE(a < b);
}

TYPED_TEST(StrongAliasTest, ComparesCorrectlyToDifferentValue) {
  using FooAlias   = StrongAlias<class FooTag, TypeParam>;
  const FooAlias a = FooAlias(ConstructValue<TypeParam>(9));
  const FooAlias b = FooAlias(ConstructValue<TypeParam>(12));
  EXPECT_NE(a, b);
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(b >= a);
  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(b > a);
  EXPECT_TRUE(a < b);
}

TEST(StrongAliasTest, CanBeDerivedFrom) {
  // Aliases can be enriched by custom operations or validations if needed.
  // Ideally, one could go from a 'using' declaration to a derived class to add
  // those methods without the need to change any other code.
  class CountryCode : public StrongAlias<CountryCode, std::string> {
   public:
    explicit CountryCode(const std::string& value) :
      StrongAlias<CountryCode, std::string>::StrongAlias(value) {
      if (this->value().length() != 2) {
        // Country code invalid!
        this->value().clear();    // is_null() will return true.
      }
    }

    [[nodiscard]]
    auto is_null() const -> bool {
      return value().empty();
    }
  };

  CountryCode valid("US");
  EXPECT_FALSE(valid.is_null());

  CountryCode invalid("United States");
  EXPECT_TRUE(invalid.is_null());
}

TEST(StrongAliasTest, CanWrapComplexStructures) {
  // A pair of strings implements odering and can, in principle, be used as
  // a base of StrongAlias.
  using PairOfStrings = std::pair<std::string, std::string>;
  using ComplexAlias  = StrongAlias<class FooTag, PairOfStrings>;

  ComplexAlias a1{std::make_pair("aaa", "bbb")};
  ComplexAlias a2{std::make_pair("ccc", "ddd")};
  EXPECT_TRUE(a1 < a2);

  EXPECT_TRUE(a1.value() == PairOfStrings("aaa", "bbb"));

  // Note a caveat, an std::pair doesn't have an overload of operator<<, and it
  // cannot be easily added since ADL rules would require it to be in the std
  // namespace. So we can't print ComplexAlias.
}

TYPED_TEST(StrongAliasTest, CanBeKeysInStdUnorderedMap) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  std::unordered_map<FooAlias, std::string, typename FooAlias::Hasher> map;

  FooAlias k1(ConstructValue<TypeParam>(0));
  FooAlias k2(ConstructValue<TypeParam>(1));

  map[k1] = "value1";
  map[k2] = "value2";

  EXPECT_EQ(map[k1], "value1");
  EXPECT_EQ(map[k2], "value2");
}

TYPED_TEST(StrongAliasTest, CanBeKeysInStdMap) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  std::map<FooAlias, std::string> map;

  FooAlias k1(ConstructValue<TypeParam>(0));
  FooAlias k2(ConstructValue<TypeParam>(1));

  map[k1] = "value1";
  map[k2] = "value2";

  EXPECT_EQ(map[k1], "value1");
  EXPECT_EQ(map[k2], "value2");
}

TYPED_TEST(StrongAliasTest, CanDifferentiateOverloads) {
  using FooAlias = StrongAlias<class FooTag, TypeParam>;
  using BarAlias = StrongAlias<class BarTag, TypeParam>;

  class Scope {
   public:
    static auto Overload(FooAlias /*  */) -> std::string { return "FooAlias"; }

    static auto Overload(BarAlias /*  */) -> std::string { return "BarAlias"; }
  };

  EXPECT_EQ("FooAlias", Scope::Overload(FooAlias()));
  EXPECT_EQ("BarAlias", Scope::Overload(BarAlias()));
}

TEST(StrongAliasTest, EnsureConstexpr) {
  using FooAlias = StrongAlias<class FooTag, int>;
  using BarAlias = StrongAlias<class BarTag, std::string_view>;

  // Check constructors.
  static constexpr FooAlias kZero{};
  static constexpr FooAlias kOne(1);
  static constexpr BarAlias kHello("Hello");

  // Check operator->.
  static_assert(kHello->size() == 5);

  // Check operator*.
  static_assert(*kZero == 0);
  static_assert(*kOne == 1);
  static_assert(*kHello == "Hello");

  // Check value().
  static_assert(kZero.value() == 0);
  static_assert(kOne.value() == 1);

  // Check explicit conversions to underlying type.
  static_assert(static_cast<int>(kZero) == 0);
  static_assert(static_cast<int>(kOne) == 1);

  // Check comparison operations.
  static_assert(kZero == kZero);
  static_assert(kZero != kOne);
  static_assert(kZero < kOne);
  static_assert(kZero <= kOne);
  static_assert(kOne > kZero);
  static_assert(kOne >= kZero);
}

}    // namespace longlp::base

// NOLINTEND(misc-redundant-expression)
