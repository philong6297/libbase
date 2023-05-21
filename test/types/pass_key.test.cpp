// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include <base/types/pass_key.h>

#include <gtest/gtest.h>

namespace longlp::base {
class Manager;

// May not be created without a PassKey.
class Restricted {
 public:
  explicit Restricted(PassKey<Manager> /* passkey */) {}
};

class Manager {
 public:
  explicit Manager() :
    restricted_(PassKey<Manager>{}) {}

 private:
  Restricted restricted_;
};

// This will not compile
// class DisallowedManager {
//  public:
//   explicit DisallowedManager() :
//     restricted_(base::PassKey<Manager>()) {}

//  private:
//   Restricted restricted_;
// };

// If this file compiles, then these test will run and pass. This is useful
// for verifying that the file actually was compiled into the unit test binary.

TEST(PassKeyTest, ExplicitConstruction) {
  Manager manager{};
}
}    // namespace longlp::base
