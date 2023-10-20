# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.20.0) # cmake_path()
project(dev_essential_tests)

set(CMAKE_CXX_EXTENSIONS OFF)   # we want standard compliant code without compiler extensions
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(../scripts/cmake/add_test_overwrite.cmake)
include(../scripts/cmake/set_position_independent_code.cmake)
include(../scripts/cmake/set_symbol_visibility.cmake)

# Generator expressions take 0 as FALSE/OFF and 1 as TRUE/ON
set(dev_essential_cmake_enable_integrated_tests 0 CACHE BOOL "" FORCE)
set(dev_essential_cmake_include_components "." CACHE STRING "List of components to test, separated\
 by whitespace or semicolon (default: '.', meaning all components are tested).")

find_package(GTest 1.10.0 REQUIRED) # INSTANTIATE_TEST_SUITE_P added in 1.10.0
enable_testing()
include(GoogleTest)

# Set compile options globally for all tests
include(../scripts/cmake/set_cxx_warn_levels.cmake)
add_compile_options(${dev_essential_cmake_cxx_warn_flags})

foreach(_component ${dev_essential_cmake_include_components})
    add_subdirectory(function/${_component})
endforeach()
add_subdirectory(sca)
