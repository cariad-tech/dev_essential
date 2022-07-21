# Copyright @ 2021 VW Group. All rights reserved.
#
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
#
# You may add additional accurate notices of copyright ownership.

cmake_minimum_required(VERSION 3.17.0)   #minimum for gtest_main targets and generated find scripts
project(dev_essential_tests)

include(../scripts/cmake/set_position_independent_code.cmake)
set(CMAKE_CXX_EXTENSIONS OFF)   # we want standard compliant code without compiler extensions
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Generator expressions take 0 as FALSE/OFF and 1 as TRUE/ON
set(dev_essential_cmake_enable_integrated_tests 0 CACHE BOOL "" FORCE)
set(dev_essential_cmake_include_components "." CACHE STRING "List of components to test, separated\
 by whitespace or semicolon (default: '.', meaning all components are tested).")

find_package(GTest REQUIRED)

##functional test integration, also includes googletest via redirection
enable_testing()

# Set compile options globally for all tests
include(../scripts/cmake/set_cxx_warn_levels.cmake)
add_compile_options(${dev_essential_cmake_cxx_warn_flags})

foreach(_component ${dev_essential_cmake_include_components})
    add_subdirectory(function/${_component})
endforeach()
add_subdirectory(sca)
