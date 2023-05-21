// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_MACROS_H_
#define LONGLP_INCLUDE_BASE_MACROS_H_

// This file define a set of macro helpers that are commonly used.

#define LONGLP_STRINGIZE(x) #x

// stringize the result of expansion of a macro arugment
#define LONGLP_EXPAND_THEN_STRINGIZE(x) LONGLP_STRINGIZE(x)

#endif    // LONGLP_INCLUDE_BASE_MACROS_H_
