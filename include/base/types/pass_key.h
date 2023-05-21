// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_TYPES_PASS_KEY_H_
#define LONGLP_INCLUDE_BASE_TYPES_PASS_KEY_H_

#include <concepts>
#include <type_traits>

namespace longlp::base {
// base::PassKey can be used to restrict access to functions to an authorized
// caller.
//
// For example, to limit the function Foo::DoSomething to the 'Manager' class:
//
//  class Foo {
//   public:
//    void DoSomething(base::PassKey<Manager>);
//  };
//
//  class Manager {
//   public:
//    using PassKey = base::PassKey<Manager>;
//    void AccessToFoo() {
//       foo_.DoSomething(PassKey());
//    }
//
//   private:
//    Foo foo_;
//  };
//
// In the above example, the 'Foo::DoSomething' requires an instance
// of base::PassKey<Manager>. Only Manager is allowed to create such instances,
// making the constructor unusable elsewhere.
//
// This method is effectively free at runtime - a few extra bytes of argument
// space are used to pass in the Passkey object.
// It is encouraged to leave the PassKey parameter unnamed to reinforce that
// it carries no semantic information and is not actually used for anything.

template <typename T>
requires std::is_class_v<T>
class PassKey {
 private:
  // use explicit to disallow creation by uniform initialization.
  explicit PassKey() = default;

  friend T;
};
}    // namespace longlp::base

#endif    // LONGLP_INCLUDE_BASE_TYPES_PASS_KEY_H_
