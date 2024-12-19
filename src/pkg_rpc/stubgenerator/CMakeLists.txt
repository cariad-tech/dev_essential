# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(clipp REQUIRED COMPONENTS clipp)

add_executable(jsonrpcstub $<TARGET_OBJECTS:libjson-rpc-cpp-stubgenerator>
                           $<TARGET_OBJECTS:libjson-rpc-cpp>
                           $<TARGET_OBJECTS:jsoncpp>
                           commandline.h
                           commandline.cpp
                           stubgeneratorfactory.cpp)

get_target_property(LIBJSON_RPC_CPP_STUBGENERATOR_INCLUDE_DIRECTORIES libjson-rpc-cpp-stubgenerator
                    INCLUDE_DIRECTORIES)
target_include_directories(jsonrpcstub
                           PRIVATE ${LIBJSON_RPC_CPP_STUBGENERATOR_INCLUDE_DIRECTORIES})
target_link_libraries(jsonrpcstub PRIVATE dev_essential::strings clipp::clipp)

find_package(Threads REQUIRED)
target_link_libraries(jsonrpcstub PRIVATE Threads::Threads)

set_target_properties(jsonrpcstub PROPERTIES FOLDER rpc/jsonrpcstub)

set_target_properties(jsonrpcstub PROPERTIES
    FOLDER rpc/jsonrpcstub
    DEBUG_POSTFIX "d"
    COMPILE_PDB_NAME_DEBUG "jsonrpcstub"
    COMPILE_PDB_NAME_RELWITHDEBINFO "jsonrpcstub"
    COMPILE_PDB_NAME_RELEASE "jsonrpcstub"
    COMPILE_PDB_NAME_MINSIZEREL "jsonrpcstub"
)

if(MSVC)
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if (is_multi_config)
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/jsonrpcstub$<$<CONFIG:Debug>:d>.pdb")
    else()
        set(pdb_file_name "${CMAKE_CURRENT_BINARY_DIR}/jsonrpcstub$<$<CONFIG:Debug>:d>.pdb")
    endif()

    install(
        FILES
            "${pdb_file_name}"
        DESTINATION
            "bin"
        CONFIGURATIONS
            "Debug" "RelWithDebInfo"
    )
endif()

install(TARGETS jsonrpcstub
        DESTINATION bin
        CONFIGURATIONS Release RelWithDebInfo Debug)
install(FILES ${dev_essential_SOURCE_DIR}/scripts/cmake/stub_generation.cmake DESTINATION cmake)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/scripts/cmake/modules DESTINATION cmake)
