// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/types/id_type.h>

#include <gtest/gtest.h>
#include <limits>

namespace longlp::base {

namespace {

  class Foo;
  using FooId = IdType<Foo, int, 0>;

}    // namespace

TEST(IdType, DefaultValueIsInvalid) {
  FooId foo_id;
  EXPECT_TRUE(foo_id.is_null());
}

TEST(IdType, NormalValueIsValid) {
  FooId foo_id = FooId::FromUnsafeValue(123);
  EXPECT_FALSE(foo_id.is_null());
}

TEST(IdType, Generator) {
  FooId::Generator foo_id_generator;
  for (int i = 1; i < 10; ++i) {
    EXPECT_EQ(foo_id_generator.GenerateNextId(), FooId::FromUnsafeValue(i));
  }
}

TEST(IdType, GeneratorWithNonZeroInvalidValue) {
  using TestId = IdType<class TestIdTag, int, -1>;

  TestId::Generator test_id_generator;
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(test_id_generator.GenerateNextId(), TestId::FromUnsafeValue(i));
  }
}

TEST(IdType, GeneratorWithBigUnsignedInvalidValue) {
  using TestId =
    IdType<class TestIdTag, uint32_t, std::numeric_limits<uint32_t>::max()>;

  TestId::Generator test_id_generator;
  for (auto i = 0U; i < 10U; ++i) {
    TestId id = test_id_generator.GenerateNextId();
    EXPECT_FALSE(id.is_null());
    EXPECT_EQ(id, TestId::FromUnsafeValue(i));
  }
}

TEST(IdType, GeneratorWithDifferentStartingValue) {
  using TestId = IdType<class TestIdTag, int, -1, 1>;

  TestId::Generator test_id_generator;
  for (int i = 1; i < 10; ++i) {
    EXPECT_EQ(test_id_generator.GenerateNextId(), TestId::FromUnsafeValue(i));
  }
}

TEST(IdType, EnsureConstexpr) {
  using TestId = IdType32<class TestTag>;

  // Test constructors.
  static constexpr TestId kZero;
  static constexpr auto kOne = TestId::FromUnsafeValue(1);

  // Test getting the underlying value.
  static_assert(kZero.value() == 0);
  static_assert(kOne.value() == 1);
  static_assert(kZero.GetUnsafeValue() == 0);
  static_assert(kOne.GetUnsafeValue() == 1);

  // Test is_null().
  static_assert(kZero.is_null());
  static_assert(!kOne.is_null());

  // Test operator bool.
  static_assert(!kZero);
  static_assert(kOne);
}

class IdTypeSpecificValueTest : public ::testing::TestWithParam<int> {
 public:
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto test_id() -> FooId { return FooId::FromUnsafeValue(GetParam()); }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  auto other_id() -> FooId {
    if (GetParam() != std::numeric_limits<int>::max()) {
      return FooId::FromUnsafeValue(GetParam() + 1);
    }
    return FooId::FromUnsafeValue(std::numeric_limits<int>::min());
  }
};

TEST_P(IdTypeSpecificValueTest, ComparisonToSelf) {
  EXPECT_TRUE(test_id() == test_id());
  EXPECT_FALSE(test_id() != test_id());
  EXPECT_FALSE(test_id() < test_id());
}

TEST_P(IdTypeSpecificValueTest, ComparisonToOther) {
  EXPECT_FALSE(test_id() == other_id());
  EXPECT_TRUE(test_id() != other_id());
}

TEST_P(IdTypeSpecificValueTest, UnsafeValueRoundtrips) {
  int original_value = GetParam();
  FooId id           = FooId::FromUnsafeValue(original_value);
  int final_value    = id.GetUnsafeValue();
  EXPECT_EQ(original_value, final_value);
}

TEST_P(IdTypeSpecificValueTest, Copying) {
  FooId original = test_id();

  FooId copy_via_constructor(original);
  EXPECT_EQ(original, copy_via_constructor);

  FooId copy_via_assignment;
  copy_via_assignment = original;
  EXPECT_EQ(original, copy_via_assignment);
}

TEST_P(IdTypeSpecificValueTest, StdMap) {
  std::map<FooId, std::string> map;

  map[test_id()]  = "test_id";
  map[other_id()] = "other_id";

  EXPECT_EQ(map[test_id()], "test_id");
  EXPECT_EQ(map[other_id()], "other_id");
}

INSTANTIATE_TEST_SUITE_P(
  All,
  IdTypeSpecificValueTest,
  ::testing::Values(
    std::numeric_limits<int>::min(),
    -1,
    0,
    1,
    123,
    std::numeric_limits<int>::max()));

}    // namespace longlp::base
