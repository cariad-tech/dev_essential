# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

## interface target for a_util_preprocessor
add_library(preprocessor INTERFACE)
target_link_libraries(preprocessor INTERFACE base)

# 3rdparty include
cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/../../extern"
           TO_CMAKE_PATH_LIST _third_party_path
           NORMALIZE)
target_include_directories(preprocessor
                           INTERFACE $<BUILD_INTERFACE:${_third_party_path}>)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/preprocessor DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/preprocessor.h DESTINATION include/a_util)
