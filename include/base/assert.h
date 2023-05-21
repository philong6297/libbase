// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_ASSERT_H_
#define LONGLP_INCLUDE_BASE_ASSERT_H_

#include <gsl/assert>

// Dont mess with Ensures and Expects from GSL
#define LONGLP_EXPECTS(cond) GSL_CONTRACT_CHECK("Precondition", cond)
#define LONGLP_ENSURES(cond) GSL_CONTRACT_CHECK("Postcondition", cond)

#endif    // LONGLP_INCLUDE_BASE_ASSERT_H_
