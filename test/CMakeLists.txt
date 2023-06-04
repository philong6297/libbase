# ---- Dependencies ----
include(CTest)
enable_testing()

if(LONGLP_IS_MASTER_PROJECT AND BASE_INSTALL)
  # base-config.cmake should be found in build dir
  find_package(base CONFIG REQUIRED PATHS ${PROJECT_BINARY_DIR})
endif()
find_package(GTest 1.12 CONFIG REQUIRED)
# cmake-format: off
find_package(
  Boost 1.82 REQUIRED
  COMPONENTS
  # Header-only libraries don't have designated components
  # dll
)
# cmake-format: on
include(GoogleTest)

# ---- Tests ----

# immediate_crash_test_helper
add_library(
  immediate_crash_test_helper_shared SHARED immediate_crash_test_helper.cpp
)
target_link_libraries(immediate_crash_test_helper_shared PRIVATE base::base)
target_compile_features(immediate_crash_test_helper_shared PRIVATE cxx_std_20)
target_compile_definitions(
  immediate_crash_test_helper_shared
  PRIVATE IMMEDIATE_CRASH_TEST_HELPER_IMPLEMENTATION
  INTERFACE BUILD_SHARED_LIBS
)

set(test_cases
    # /
    auto_reset
    no_destructor
    assert
    # types/
    types/pass_key
    types/strong_alias
    types/id_type
    # containers/
    containers/vector_buffer
    # strings/
    strings/utf_string_conversion_utils
    strings/string_utils.compare_case_insensitive_ascii
    strings/string_utils.equals_case_insensitive_ascii
    strings/string_utils.remove_chars
    strings/string_utils.replace_chars
    strings/string_utils.to_lower_ascii
    strings/string_utils.to_upper_ascii
    strings/string_utils.trim_string
    # icu/
    icu/utf.utf8
    icu/utf.utf16
)
list(TRANSFORM test_cases APPEND .test.cpp)

set(test_utils
    copy_only_int.h copy_only_int.cpp move_only_int.h
    gtest_fix_u8string_comparison.h gtest_fix_u8string_comparison.cpp
)
list(TRANSFORM test_utils PREPEND test_utils/)

add_executable(base_test)
target_sources(base_test PRIVATE ${test_cases} ${test_utils})
# TODO(longlp, vcpkg-issue): vcpkg did not provide target for Boost.DLL
target_link_libraries(
  base_test PRIVATE base::base GTest::gtest_main Boost::boost
)
target_compile_features(base_test PRIVATE cxx_std_20)
target_include_directories(base_test PRIVATE ${PROJECT_SOURCE_DIR}/test)
target_compile_definitions(
  base_test
  PRIVATE
    IMMEDIATE_CRASH_HELPER_LIB_DIR=$<TARGET_FILE:immediate_crash_test_helper_shared>
)
add_dependencies(base_test immediate_crash_test_helper_shared)
if(LONGLP_IS_MASTER_PROJECT AND LONGLP_DEV_MODE)
  set(options ${LONGLP_DESIRED_COMPILE_OPTIONS})
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(
      REMOVE_ITEM
      options
      -Wglobal-constructors # disable warnings on GTest macros
      -Wweak-vtables # disable warning on declare inline special member function
                     # in class
    )
    list(
      APPEND
      options
      -Wno-global-constructors # disable warnings on GTest macros
      -Wno-weak-vtables # disable warning on declare inline special member
                        # function in class
    )
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(REMOVE_ITEM options -fanalyzer # GCC cannot analyze TYPED_TEST_* and
                                        # INSTANTIATE_TEST_SUITE_P due to its
                                        # recursive complex
    )
    list(APPEND options -fno-analyzer # GCC cannot analyze TYPED_TEST_* and
                                      # INSTANTIATE_TEST_SUITE_P due to its
                                      # recursive complex
    )
  endif()
  target_compile_options(base_test PRIVATE ${options})
endif()

gtest_discover_tests(base_test)
