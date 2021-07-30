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

add_library(datetime STATIC
            ../../include/a_util/datetime.h
            ../../include/a_util/datetime/datetime.h
            datetime.cpp
            )
target_link_libraries(datetime PUBLIC base
                               PRIVATE regex)
set_target_properties(datetime PROPERTIES FOLDER a_util
                                          OUTPUT_NAME a_util_datetime)
install(TARGETS datetime)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/datetime DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/datetime.h DESTINATION include/a_util)
