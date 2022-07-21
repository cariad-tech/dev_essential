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

## interface target for a_util_preprocessor
add_library(preprocessor INTERFACE)
target_link_libraries(preprocessor INTERFACE base)

# 3rdparty include
get_filename_component(_third_party_path "${CMAKE_CURRENT_LIST_DIR}/../../extern" ABSOLUTE)
target_include_directories(preprocessor
                           INTERFACE $<BUILD_INTERFACE:${_third_party_path}>)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/preprocessor DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/preprocessor.h DESTINATION include/a_util)
