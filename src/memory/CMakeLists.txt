# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

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

set_target_properties(memory PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_memory
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_memoryd"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_memory"
    COMPILE_PDB_NAME_RELEASE "a_util_memory"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_memory"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_memory$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_memory$<$<CONFIG:Debug>:d>.pdb")
    endif()

    install(
        FILES
            "${pdb_file_name}"
        DESTINATION
            "lib"
        CONFIGURATIONS
            "Debug" "RelWithDebInfo"
    )
endif()
install(TARGETS memory)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/memory DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/memory.h DESTINATION include/a_util)
