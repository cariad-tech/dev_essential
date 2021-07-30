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

add_library(filesystem STATIC
            ../../include/a_util/filesystem.h
            ../../include/a_util/filesystem/filesystem.h
            ../../include/a_util/filesystem/path.h
            path.cpp   #common implementation
            filesystem.cpp   #common implementation
            )
target_link_libraries(filesystem PUBLIC base
                                 PRIVATE strings)
set_target_properties(filesystem PROPERTIES FOLDER a_util
                                            OUTPUT_NAME a_util_filesystem)
install(TARGETS filesystem)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/filesystem DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/filesystem.h DESTINATION include/a_util)
