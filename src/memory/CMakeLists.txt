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

add_library(memory STATIC
    ../../include/a_util/memory.h
    ../../include/a_util/memory/memory.h
    ../../include/a_util/memory/memorybuffer.h
    ../../include/a_util/memory/unique_ptr.h
    ../../include/a_util/memory/shared_ptr.h
    ../../include/a_util/memory/stack_ptr.h
    ../../include/a_util/memory/detail/stack_ptr_decl.h
    ../../include/a_util/memory/detail/stack_ptr_impl.h
    memory.cpp
    memorybuffer.cpp
    )
target_link_libraries(memory PUBLIC base
                             PRIVATE strings)
set_target_properties(memory PROPERTIES FOLDER a_util
                                        OUTPUT_NAME a_util_memory)
install(TARGETS memory)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/memory DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/memory.h DESTINATION include/a_util)
