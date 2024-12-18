# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(variant STATIC
            ../../include/a_util/variant.h
            ../../include/a_util/variant/variant.h
            variant.cpp   #common implementation
            )
target_link_libraries(variant PUBLIC base
                              PRIVATE memory)

set_target_properties(variant PROPERTIES 
    FOLDER a_util
    OUTPUT_NAME a_util_variant
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "a_util_variantd"
    COMPILE_PDB_NAME_RELWITHDEBINFO "a_util_variant"
    COMPILE_PDB_NAME_RELEASE "a_util_variant"
    COMPILE_PDB_NAME_MINSIZEREL "a_util_variant"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/a_util_variant$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/a_util_variant$<$<CONFIG:Debug>:d>.pdb")
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
install(TARGETS variant)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/a_util/variant DESTINATION include/a_util)
install(FILES ${dev_essential_SOURCE_DIR}/include/a_util/variant.h DESTINATION include/a_util)
