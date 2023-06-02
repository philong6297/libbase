// Copyright 2023 Phi-Long Le. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef LONGLP_INCLUDE_BASE_IMMEDIATE_CRASH_H_
#define LONGLP_INCLUDE_BASE_IMMEDIATE_CRASH_H_

#include "base/compiler_specific.h"
#include "base/predef.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-do-while, hicpp-no-assembler)

// Crashes in the fastest possible way with no attempt at logging.
// There are several constraints; see http://crbug.com/664209 for more context.
//
// - LONGLP_TRAP_SEQUENCE_() must be fatal. It should not be possible to ignore
// the
//   resulting exception or simply hit 'continue' to skip over it in a debugger.
// - Different instances of LONGLP_TRAP_SEQUENCE_() must not be folded together,
// to
//   ensure crash reports are debuggable. Unlike __builtin_trap(), asm volatile
//   blocks will not be folded together.
//   Note: LONGLP_TRAP_SEQUENCE_() previously required an instruction with a
//   unique nonce since unlike clang, GCC folds together identical asm volatile
//   blocks.
// - LONGLP_TRAP_SEQUENCE_() must produce a signal that is distinct from an
// invalid
//   memory access.
// - LONGLP_TRAP_SEQUENCE_() must be treated as a set of noreturn instructions.
//   __builtin_unreachable() is used to provide that hint here. clang also uses
//   this as a heuristic to pack the instructions in the function epilogue to
//   improve code density.
// - base::ImmediateCrash() is used in allocation hooks. To prevent recursions,
//   LONGLP_TRAP_SEQUENCE_() must not allocate.
//
// Additional properties that are nice to have:
// - LONGLP_TRAP_SEQUENCE_() should be as compact as possible.
// - The first instruction of LONGLP_TRAP_SEQUENCE_() should not change, to
// avoid
//   shifting crash reporting clusters. As a consequence of this, explicit
//   assembly is preferred over intrinsics.
//   Note: this last bullet point may no longer be true, and may be removed in
//   the future.

// Note: TRAP_SEQUENCE Is currently split into two macro helpers due to the fact
// that clang emits an actual instruction for __builtin_unreachable() on certain
// platforms (see https://crbug.com/958675). In addition, the int3/bkpt/brk will
// be removed in followups, so splitting it up like this now makes it easy to
// land the followups.

#if defined(LONGLP_OS_LINUX) && \
  (defined(LONGLP_COMPILER_CLANG) || defined(LONGLP_COMPILER_GCC))

#  if defined(LONGLP_ARCH_CPU_X86_FAMILY)    // Intel x86 family (x86/x86-64) in
                                             // Linux + GCC/Clang

// TODO(https://crbug.com/958675): In theory, it should be possible to use just
// int3. However, there are a number of crashes with SIGILL as the exception
// code, so it seems likely that there's a signal handler that allows execution
// to continue after SIGTRAP.
#    define LONGLP_TRAP_SEQUENCE1_() asm volatile("int3")
// TODO(longlp, support): Apple for LONGLP_TRAP_SEQUENCE2_
#    define LONGLP_TRAP_SEQUENCE2_() asm volatile("ud2")

#  elif defined(LONGLP_ARCH_CPU_ARM64)    // ARM64 in Linux + GCC/Clang

// This will always generate a SIGTRAP on arm64.
// TODO(https://crbug.com/958675): Remove brk from this sequence.
#    define LONGLP_TRAP_SEQUENCE1_() asm volatile("brk #0")
#    define LONGLP_TRAP_SEQUENCE2_() asm volatile("hlt #0")

#  else    // Other architectures in linux + GCC/Clang

// Crash report accuracy will not be guaranteed on other architectures, but at
// least this will crash as expected.
#    define LONGLP_TRAP_SEQUENCE1_() __builtin_trap()
#    define LONGLP_TRAP_SEQUENCE2_() asm volatile("")

#  endif

#elif defined(LONGLP_OS_WINDOWS) && \
  (defined(LONGLP_COMPILER_CLANG) || defined(LONGLP_COMPILER_MSVC))

#  if defined(LONGLP_COMPILER_MSVC)    // Windows + MSVC

// MSVC x64 doesn't support inline asm, so use the MSVC intrinsic.
#    define LONGLP_TRAP_SEQUENCE1_() __debugbreak()
#    define LONGLP_TRAP_SEQUENCE2_()

#  else    // Windows + Clang

#    if defined(LONGLP_ARCH_CPU_ARM64)    // ARM64 in Windows + Clang
// Windows ARM64 uses "BRK #F000" as its breakpoint instruction, and
// __debugbreak() generates that in both VC++ and clang.
#      define LONGLP_TRAP_SEQUENCE1_() __debugbreak()
// Intentionally empty: __builtin_unreachable() is always part of the sequence
// (see IMMEDIATE_CRASH below) and already emits a ud2 on Win64,
// https://crbug.com/958373
#      define LONGLP_TRAP_SEQUENCE2_() __asm volatile("")

#    else    // Other architectures in Windows + Clang

#      define LONGLP_TRAP_SEQUENCE1_() asm volatile("int3")
#      define LONGLP_TRAP_SEQUENCE2_() asm volatile("ud2")

#    endif
#  endif

#else
#  error No supported trap sequence!
#endif

#define LONGLP_TRAP_SEQUENCE_() \
  do {                          \
    LONGLP_TRAP_SEQUENCE1_();   \
    LONGLP_TRAP_SEQUENCE2_();   \
  }                             \
  while (false)

namespace longlp::base {

[[noreturn]]
LONGLP_ALWAYS_INLINE void ImmediateCrash() {
  LONGLP_TRAP_SEQUENCE_();
#if defined(LONGLP_COMPILER_CLANG) || defined(LONGLP_COMPILER_GCC)
  __builtin_unreachable();
#endif    // defined(LONGLP_COMPILER_CLANG) || defined(LONGLP_COMPILER_GCC)
}

}    // namespace longlp::base

// NOLINTEND(cppcoreguidelines-avoid-do-while, hicpp-no-assembler)

#endif    // LONGLP_INCLUDE_BASE_IMMEDIATE_CRASH_H_
