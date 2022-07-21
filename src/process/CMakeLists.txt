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

add_library(process STATIC
            ../../include/a_util/process.h
            ../../include/a_util/process/process.h
            ../../include/a_util/process/detail/process_impl.h
            process.cpp
            process_win.cpp
            process_lin.cpp
            )
target_link_libraries(process PUBLIC base
                              PRIVATE system)
if(UNIX AND NOT QNXNTO)
    target_link_libraries(process PUBLIC rt)
endif()
if(WIN32)
    target_link_libraries(process PUBLIC winmm)
endif()
set_target_properties(process PROPERTIES FOLDER a_util
                                         OUTPUT_NAME a_util_process)
install(TARGETS process)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/process DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/process.h DESTINATION include/a_util)
