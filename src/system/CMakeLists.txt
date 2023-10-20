# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_subdirectory(../../extern/3rdparty/uuid ${CMAKE_BINARY_DIR}/extern/3rdparty/uuid)

add_library(system STATIC
            ../../include/a_util/system.h
            ../../include/a_util/system/address_info.h
            ../../include/a_util/system/system.h
            ../../include/a_util/system/timer.h
            ../../include/a_util/system/timer_decl.h
            ../../include/a_util/system/uuid.h
            ../../include/a_util/system/detail/timer_impl.h
            address_info.cpp
            system.cpp
            timer.cpp
            uuid.cpp
            $<TARGET_OBJECTS:uuid>
            )
target_include_directories(system PRIVATE $<BUILD_INTERFACE:${UUID_DIR}>)
target_link_libraries(system PUBLIC base
                             PRIVATE filesystem
                                     concurrency
                                     $<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS}>)

if(UNIX AND NOT QNXNTO)
    target_link_libraries(system PUBLIC rt)
endif()
if(WIN32)
    target_link_libraries(system PUBLIC winmm)
endif()

set_target_properties(system PROPERTIES FOLDER a_util
                                        OUTPUT_NAME a_util_system)

install(TARGETS system)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/system DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/system.h DESTINATION include/a_util)
