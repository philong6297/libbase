// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/assert.h>

#include <gtest/gtest.h>
#include <string_view>

namespace longlp::base {
namespace {

  constexpr std::string_view kDeathString                   = "Expected Death";
  constexpr std::string_view kFailedSetTerminateDeathstring = ".*";

  // This prevents a failed call to set_terminate from failing the test suite.
  constexpr auto
  GetExpectedDeathString(std::terminate_handler handle) -> const char* {
    return handle != nullptr
           ? kDeathString.data()
           : kFailedSetTerminateDeathstring.data();
  }

  auto FunctionWithPrecondition(int value) -> int {
    LONGLP_EXPECTS(value > 0 && value < 10);
    return value;
  }

  auto FunctionWithPostCondition(int value) -> int {
    ++value;
    LONGLP_ENSURES(value > 0 && value < 10);
    return value;
  }
}    // namespace

TEST(AssertTest, Expects) {
  const auto terminate_handler = std::set_terminate([] {
    std::cerr << "Expected Death. expects";
    std::abort();
  });

  EXPECT_TRUE(FunctionWithPrecondition(2) == 2);
  EXPECT_DEATH(
    FunctionWithPrecondition(10),
    GetExpectedDeathString(terminate_handler));
}

TEST(AssertTest, Ensures) {
  const auto terminate_handler = std::set_terminate([] {
    std::cerr << "Expected Death. ensures";
    std::abort();
  });

  EXPECT_TRUE(FunctionWithPostCondition(2) == 3);
  EXPECT_DEATH(
    FunctionWithPostCondition(9),
    GetExpectedDeathString(terminate_handler));
}
}    // namespace longlp::base
