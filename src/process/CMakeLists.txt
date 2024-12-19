# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

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

set_target_properties(process PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_process
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_processd"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_process"
    COMPILE_PDB_NAME_RELEASE "a_util_process"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_process"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_process$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_process$<$<CONFIG:Debug>:d>.pdb")
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
install(TARGETS process)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/process DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/process.h DESTINATION include/a_util)
