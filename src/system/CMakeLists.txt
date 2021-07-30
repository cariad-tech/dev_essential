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

add_subdirectory(../../extern/3rdparty/uuid ${CMAKE_BINARY_DIR}/extern/3rdparty/uuid)

add_library(system STATIC
            ../../include/a_util/system.h
            ../../include/a_util/system/system.h
            ../../include/a_util/system/timer.h
            ../../include/a_util/system/timer_decl.h
            ../../include/a_util/system/uuid.h
            ../../include/a_util/system/detail/timer_impl.h
            system.cpp
            timer.cpp
            uuid.cpp
            $<TARGET_OBJECTS:uuid>
            )
target_include_directories(system PRIVATE $<BUILD_INTERFACE:${UUID_DIR}>)
target_link_libraries(system PUBLIC base
                             PRIVATE filesystem)

if(UNIX AND NOT QNXNTO)
    target_link_libraries(system PUBLIC rt)
endif()
if(WIN32)
    target_link_libraries(system PUBLIC winmm)
endif()

set_target_properties(system PROPERTIES FOLDER a_util
                                        OUTPUT_NAME a_util_system)

install(TARGETS system)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/a_util/system DESTINATION include/a_util)
install(FILES ${CMAKE_SOURCE_DIR}/include/a_util/system.h DESTINATION include/a_util)
