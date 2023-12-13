# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

## interface target for dev_essential::base
add_library(base INTERFACE)
target_compile_features(base INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_include_directories(base INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                          $<INSTALL_INTERFACE:include>)

include(../../scripts/cmake/set_cxx_warn_levels.cmake)
target_compile_options(base INTERFACE $<BUILD_INTERFACE:${dev_essential_cmake_cxx_warn_flags}>)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/base DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/base.h DESTINATION include/a_util)
