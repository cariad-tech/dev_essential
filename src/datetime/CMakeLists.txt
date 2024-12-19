# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(datetime STATIC
            ../../include/a_util/datetime.h
            ../../include/a_util/datetime/datetime.h
            datetime.cpp
            )
target_link_libraries(datetime PUBLIC base
                               PRIVATE regex)

set_target_properties(datetime PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_datetime
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_datetimed"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_datetime"
    COMPILE_PDB_NAME_RELEASE "a_util_datetime"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_datetime"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_datetime$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_datetime$<$<CONFIG:Debug>:d>.pdb")
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

install(TARGETS datetime)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/datetime DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/datetime.h DESTINATION include/a_util)
