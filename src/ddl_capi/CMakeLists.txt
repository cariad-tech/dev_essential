# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

add_library(ddl_capi SHARED ddl_capi.cpp
                            ../../include/ddl_capi/ddl_capi.h)

set_target_properties(ddl_capi PROPERTIES 
    FOLDER ddl_capi
    OUTPUT_NAME ddl_capi
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "ddl_capid"
    COMPILE_PDB_NAME_RELWITHDEBINFO "ddl_capi"
    COMPILE_PDB_NAME_RELEASE "ddl_capi"
    COMPILE_PDB_NAME_MINSIZEREL "ddl_capi"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ddl_capi$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/ddl_capi$<$<CONFIG:Debug>:d>.pdb")
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

target_compile_definitions(ddl_capi PUBLIC A_UTIL_DO_EXPORT)

target_link_libraries(ddl_capi PUBLIC preprocessor PRIVATE ddl)

install(TARGETS ddl_capi)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/ddl_capi DESTINATION include)

# test ddl-c-api, only for internal use
add_library(ddl_capi_c_test OBJECT ddl_capi.c)
set_target_properties(ddl_capi_c_test PROPERTIES FOLDER ddl_capi)
target_link_libraries(ddl_capi_c_test PRIVATE ddl_capi)
