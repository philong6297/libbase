# ---- Dependencies ----
include(CTest)
enable_testing()

if(LONGLP_IS_MASTER_PROJECT AND BASE_INSTALL)
  # base-config.cmake should be found in build dir
  find_package(base CONFIG REQUIRED PATHS ${PROJECT_BINARY_DIR})
endif()
find_package(GTest 1.12 CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

set(test_cases auto_reset no_destructor assert types/pass_key
               types/strong_alias types/id_type
)
list(TRANSFORM test_cases APPEND .test.cpp)

add_executable(base_test)
target_sources(base_test PRIVATE ${test_cases})
target_link_libraries(base_test PRIVATE base::base GTest::gtest_main)
target_compile_features(base_test PRIVATE cxx_std_20)
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
