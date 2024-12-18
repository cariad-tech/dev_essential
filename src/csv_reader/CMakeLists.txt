# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(csv_reader STATIC
            ../../include/a_util/parser.h
            ../../include/a_util/parser/csv_reader.h
            csv_reader.cpp
            )
target_link_libraries(csv_reader PUBLIC base)

set_target_properties(csv_reader PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_csv_reader
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_csv_readerd"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_csv_reader"
    COMPILE_PDB_NAME_RELEASE "a_util_csv_reader"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_csv_reader"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_csv_reader$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_csv_reader$<$<CONFIG:Debug>:d>.pdb")
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

install(TARGETS csv_reader)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/parser DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/parser.h DESTINATION include/a_util)
