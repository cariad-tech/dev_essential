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

## interface target for dev_essential::base
add_library(base INTERFACE)
target_compile_features(base INTERFACE cxx_std_14)
target_include_directories(base INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                          $<INSTALL_INTERFACE:include>)

include(../../scripts/cmake/set_cxx_warn_levels.cmake)
target_compile_options(base INTERFACE $<BUILD_INTERFACE:${dev_essential_cmake_cxx_warn_flags}>)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/base DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/base.h DESTINATION include/a_util)
