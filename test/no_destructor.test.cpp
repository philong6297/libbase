// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/no_destructor.h>

#include <base/predef.h>
#include <gtest/gtest.h>

namespace longlp::base {

// no_destructor should be trivially destructible
static_assert(!std::is_trivially_destructible_v<std::string>);
static_assert(
  std::is_trivially_destructible_v<base::NoDestructor<std::string>>);

struct CheckOnDestroy {
  CheckOnDestroy()                                         = default;

  CheckOnDestroy(const CheckOnDestroy&)                    = default;
  CheckOnDestroy(CheckOnDestroy&&)                         = default;
  auto operator=(const CheckOnDestroy&) -> CheckOnDestroy& = default;
  auto operator=(CheckOnDestroy&&) -> CheckOnDestroy&      = default;

  virtual ~CheckOnDestroy() {
    // it should not be checked here
    ADD_FAILURE();
  }
};

struct DisallowCopyAndMove : public CheckOnDestroy {
  DisallowCopyAndMove() = default;

  explicit DisallowCopyAndMove(int new_value) :
    value(new_value) {}

  DisallowCopyAndMove(const DisallowCopyAndMove&)                    = delete;
  auto operator=(const DisallowCopyAndMove&) -> DisallowCopyAndMove& = delete;
  ~DisallowCopyAndMove() override                                    = default;

  int value{1};
  std::string something_with_a_nontrivial_destructor{};
};

struct DisallowMove {
  DisallowMove()                                       = default;
  DisallowMove(const DisallowMove&)                    = default;
  auto operator=(const DisallowMove&) -> DisallowMove& = default;
  DisallowMove(DisallowMove&&)                         = delete;
  auto operator=(DisallowMove&&) -> DisallowMove&      = delete;
  ~DisallowMove()                                      = default;
};

struct DisallowCopy {
  DisallowCopy()                                       = default;
  DisallowCopy(const DisallowCopy&)                    = delete;
  auto operator=(const DisallowCopy&) -> DisallowCopy& = delete;
  DisallowCopy(DisallowCopy&&)                         = default;
  auto operator=(DisallowCopy&&) -> DisallowCopy&      = default;
  ~DisallowCopy()                                      = default;
};

struct ForwardingClass : public CheckOnDestroy {
  ForwardingClass(const ForwardingClass&)                    = default;
  ForwardingClass(ForwardingClass&&)                         = default;
  auto operator=(const ForwardingClass&) -> ForwardingClass& = default;
  auto operator=(ForwardingClass&&) -> ForwardingClass&      = default;

  ForwardingClass(
    const DisallowMove& /* copy_only */,
    DisallowCopy&& /* move_only */) {}

  ~ForwardingClass() override = default;

  std::string something_with_a_nontrivial_destructor{};
};

TEST(NoDestructorTest, SkipsDestructors) {
  NoDestructor<CheckOnDestroy> destructor_should_not_run;
}

TEST(NoDestructorTest, DisallowCopyAndMove) {
  static NoDestructor<DisallowCopyAndMove> default_constructed;
  EXPECT_EQ(1, default_constructed->value);

  static NoDestructor<DisallowCopyAndMove> constructed_with_arg(-1);
  EXPECT_EQ(-1, constructed_with_arg->value);
}

TEST(NoDestructorTest, ForwardsArguments) {
  DisallowMove copy_only;
  DisallowCopy move_only;

  static NoDestructor<ForwardingClass>
    test_forwarding(copy_only, std::move(move_only));
}

TEST(NoDestructorTest, Accessors) {
  static NoDestructor<std::string> awesome("awesome");

  EXPECT_EQ("awesome", *awesome);
  EXPECT_EQ(0, awesome->compare("awesome"));
  EXPECT_EQ(0, awesome.get()->compare("awesome"));
}

// Passing initializer list to a NoDestructor like in this test
// is ambiguous in GCC.
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84849
TEST(NoDestructorTest, InitializerList) {
  static NoDestructor<std::vector<std::string>> vector({"a", "b", "c"});
}

}    // namespace longlp::base
