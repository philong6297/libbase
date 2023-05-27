// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#include "test_utils/copy_only_int.h"

namespace longlp::base {

// static
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
int CopyOnlyInt::num_copies_ = 0;

}    // namespace longlp::base
