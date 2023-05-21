cmake_minimum_required(VERSION 3.26)

# Determine if project is built as a subproject (using add_subdirectory) or if
# it is the master project.
if(NOT DEFINED LONGLP_IS_MASTER_PROJECT)
  set(LONGLP_IS_MASTER_PROJECT OFF)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(LONGLP_IS_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif()
endif()

project(base LANGUAGES CXX)

# should use absolute path when dealing with CMAKE variables
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")

include(join_strings)
include(set_verbose)
include(join_paths)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# OPTIONS & CONFIGURATION ----

# Set the default CMAKE_BUILD_TYPE to Release. This should be done before the
# project command since the latter can set CMAKE_BUILD_TYPE itself (it does so
# for nmake).
if(LONGLP_IS_MASTER_PROJECT AND NOT CMAKE_BUILD_TYPE)
  set_verbose(
    CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or "
    "CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
  )
endif()

set_verbose(
  LONGLP_PROJECT_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE STRING
  "Installation directory for include files, a relative path that "
  "will be joined with ${CMAKE_INSTALL_PREFIX} or an absolute path."
)

# Options that control generation of various targets.
option(BASE_INSTALL "Generate the install target." ON)
option(ENABLE_TESTING "Generate the test target." ${LONGLP_IS_MASTER_PROJECT})
option(BASE_AS_SYSTEM_HEADERS "Expose headers with marking them as system." OFF)

set(LONGLP_SYSTEM_HEADER_ATTRIBUTE "")
if(BASE_AS_SYSTEM_HEADERS)
  set(LONGLP_SYSTEM_HEADER_ATTRIBUTE SYSTEM)
endif()

# Get version from VERSION file
file(READ VERSION version_content)
if(NOT version_content MATCHES "MAJOR=([0-9]+)\nMINOR=([0-9]+)\nBUILD=([0-9]+)")
  message(FATAL_ERROR "Cannot get BASE_VERSION from VERSION file.")
endif()
# Use math to skip leading zeros if any.
math(EXPR CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
math(EXPR CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
math(EXPR CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})
join_strings(
  BASE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.
  ${CPACK_PACKAGE_VERSION_PATCH}
)

message(STATUS "Version: ${BASE_VERSION}")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

if(LONGLP_IS_MASTER_PROJECT AND NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET)
  set_verbose(
    CMAKE_CXX_VISIBILITY_PRESET hidden CACHE STRING
    "Preset for the export of private symbols"
  )
  set_property(
    CACHE CMAKE_CXX_VISIBILITY_PRESET PROPERTY STRINGS hidden default
  )
endif()

if(LONGLP_IS_MASTER_PROJECT AND NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
  set_verbose(
    CMAKE_VISIBILITY_INLINES_HIDDEN ON CACHE BOOL
    "Whether to add a compile flag to hide symbols of inline functions"
  )
endif()

if(LONGLP_IS_MASTER_PROJECT AND CMAKE_GENERATOR MATCHES "Visual Studio")
  # If Microsoft SDK is installed create script run-msbuild.bat that calls
  # SetEnv.cmd to set up build environment and runs msbuild. It is useful when
  # building Visual Studio projects with the SDK toolchain rather than Visual
  # Studio.
  include(find_windows_set_env_cmd)
  if(LONGLP_WINSDK_SETENV)
    set(MSBUILD_SETUP "call \"${LONGLP_WINSDK_SETENV}\"")
  endif()
  # Set FrameworkPathOverride to get rid of MSB3644 warnings.
  join_strings(
    netfxpath "C:\\Program Files\\Reference Assemblies\\Microsoft\\Framework\\"
    ".NETFramework\\v4.0"
  )
  file(
    WRITE run-msbuild.bat
    "
    ${MSBUILD_SETUP}
    ${CMAKE_MAKE_PROGRAM} -p:FrameworkPathOverride=\"${netfxpath}\" %*"
  )
endif()

# DECLARE TARGETS ----

find_package(fmt 9 CONFIG REQUIRED)
find_package(Microsoft.GSL 4 CONFIG REQUIRED)
# cmake-format: off
find_package(
  Boost 1.82 REQUIRED
  COMPONENTS
  # Header-only libraries don't have designated components
  # predef
  # config
)
# cmake-format: on

set(BASE_PUBLIC_HEADERS auto_reset.h base_export.h assert.h no_destructor.h
                        predef.h
)
list(TRANSFORM BASE_PUBLIC_HEADERS PREPEND include/base/)

set(BASE_SOURCES base.cpp)
list(TRANSFORM BASE_SOURCES PREPEND src/)

add_library(base)
add_library(base::base ALIAS base)
target_sources(base PRIVATE ${BASE_SOURCES} ${BASE_PUBLIC_HEADERS})
target_compile_features(base PUBLIC cxx_std_20)
# TODO(longlp, vcpkg-issue): vcpkg did not provide target for Boost.Predef and
# Boost.Config
target_link_libraries(base PRIVATE Microsoft.GSL::GSL fmt::fmt Boost::boost)

target_include_directories(
  base ${LONGLP_SYSTEM_HEADER_ATTRIBUTE}
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${LONGLP_PROJECT_INCLUDE_DIR}>
)

set(BASE_DEBUG_POSTFIX
    d
    CACHE STRING "Debug library postfix."
)

set_target_properties(
  base
  PROPERTIES VERSION ${BASE_VERSION}
             SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}
             PUBLIC_HEADER "${BASE_PUBLIC_HEADERS}"
             DEBUG_POSTFIX "${BASE_DEBUG_POSTFIX}"
)

# Set BASE_LIB_NAME for pkg-config base.pc. We cannot use the OUTPUT_NAME target
# property because it's not set by default.
set(BASE_LIB_NAME base)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BASE_LIB_NAME ${BASE_LIB_NAME}${BASE_DEBUG_POSTFIX})
endif()

if(BUILD_SHARED_LIBS)
  target_compile_definitions(
    base
    PRIVATE BASE_IMPLEMENTATION
    INTERFACE BASE_AS_SHARED_LIB
  )
endif()

# configurations for development
if(LONGLP_IS_MASTER_PROJECT AND LONGLP_DEV_MODE)
  set(ENABLE_TESTING ON)

  include(longlp_desired_compile_options)
  longlp_desired_compile_options(LONGLP_DESIRED_COMPILE_OPTIONS)

  target_compile_options(base PRIVATE ${LONGLP_DESIRED_COMPILE_OPTIONS})
endif()

# INSTALL TARGET ----
if(BASE_INSTALL)
  include(CMakePackageConfigHelpers)
  set_verbose(
    BASE_CMAKE_INSTALL_DIR
    ${CMAKE_INSTALL_LIBDIR}/cmake/base
    CACHE
    STRING
    "Installation directory for cmake files, a relative path that "
    "will be joined with ${CMAKE_INSTALL_PREFIX} or an absolute "
    "path."
  )
  set(version_config ${PROJECT_BINARY_DIR}/base-config-version.cmake)
  set(project_config ${PROJECT_BINARY_DIR}/base-config.cmake)
  set(pkgconfig ${PROJECT_BINARY_DIR}/base.pc)
  set(targets_export_name base-targets)

  set_verbose(
    BASE_LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR} CACHE STRING
    "Installation directory for libraries, a relative path that "
    "will be joined to ${CMAKE_INSTALL_PREFIX} or an absolute path."
  )

  set_verbose(
    BASE_PKGCONFIG_INSTALL_DIR
    ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    CACHE
    PATH
    "Installation directory for pkgconfig (.pc) files, a relative "
    "path that will be joined with ${CMAKE_INSTALL_PREFIX} or an "
    "absolute path."
  )

  # Generate the version, config and target files into the build directory.
  write_basic_package_version_file(
    ${version_config}
    VERSION ${BASE_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

  join_paths(libdir_for_pc_file "\${exec_prefix}" "${BASE_LIB_INSTALL_DIR}")
  join_paths(
    includedir_for_pc_file "\${prefix}" "${LONGLP_PROJECT_INCLUDE_DIR}"
  )

  configure_file(
    "${PROJECT_SOURCE_DIR}/tools/cmake/base.pc.in" "${pkgconfig}" @ONLY
  )
  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/tools/cmake/base-config.cmake.in ${project_config}
    INSTALL_DESTINATION ${BASE_CMAKE_INSTALL_DIR}
  )

  set(INSTALL_TARGETS base)

  # Install the library and headers.
  install(
    TARGETS ${INSTALL_TARGETS}
    EXPORT ${targets_export_name}
    LIBRARY DESTINATION ${BASE_LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${BASE_LIB_INSTALL_DIR}
    PUBLIC_HEADER DESTINATION "${LONGLP_PROJECT_INCLUDE_DIR}/base"
    FRAMEWORK DESTINATION "."
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  # Use a namespace because CMake provides better diagnostics for namespaced
  # imported targets.
  export(
    TARGETS ${INSTALL_TARGETS}
    NAMESPACE base::
    FILE ${PROJECT_BINARY_DIR}/${targets_export_name}.cmake
  )

  # Install version, config and target files.
  install(FILES ${project_config} ${version_config}
          DESTINATION ${BASE_CMAKE_INSTALL_DIR}
  )
  install(
    EXPORT ${targets_export_name}
    DESTINATION ${BASE_CMAKE_INSTALL_DIR}
    NAMESPACE base::
  )

  install(FILES "${pkgconfig}" DESTINATION "${BASE_PKGCONFIG_INSTALL_DIR}")
endif()

set(gitignore ${PROJECT_SOURCE_DIR}/.gitignore)
if(LONGLP_IS_MASTER_PROJECT AND EXISTS ${gitignore})
  # Get the list of ignored files from .gitignore.
  file(STRINGS ${gitignore} lines)
  list(REMOVE_ITEM lines /doc/html)
  foreach(line ${lines})
    string(REPLACE "." "[.]" line "${line}")
    string(REPLACE "*" ".*" line "${line}")
    set(ignored_files ${ignored_files} "${line}$" "${line}/")
  endforeach()
  set(ignored_files ${ignored_files} /.git)

  set(CPACK_SOURCE_GENERATOR ZIP)
  set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
  set(CPACK_SOURCE_PACKAGE_FILE_NAME base-${BASE_VERSION})
  set(CPACK_PACKAGE_NAME base)
  set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
  set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README.md)
  include(CPack)
endif()

# Testing ----

# should be located after install phase to ensure the config file is generated
if(ENABLE_TESTING)
  add_subdirectory(test)
endif()
