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

add_executable(jsonrpcstub $<TARGET_OBJECTS:libjson-rpc-cpp-stubgenerator>
                           $<TARGET_OBJECTS:libjson-rpc-cpp>
                           $<TARGET_OBJECTS:jsoncpp>
                           stubgeneratorfactory.cpp
                           commandline.cpp
                           parserhelper.cpp
                           commandline.h
                           parserhelper.h)
get_target_property(LIBJSON_RPC_CPP_STUBGENERATOR_INCLUDE_DIRECTORIES libjson-rpc-cpp-stubgenerator
                    INCLUDE_DIRECTORIES)
target_include_directories(jsonrpcstub
                           PRIVATE ${LIBJSON_RPC_CPP_STUBGENERATOR_INCLUDE_DIRECTORIES})
target_link_libraries(jsonrpcstub PRIVATE dev_essential::strings)
if(NOT MSVC)
    find_package(Threads REQUIRED)
    target_link_libraries(jsonrpcstub PRIVATE Threads::Threads)
endif(NOT MSVC)
set_target_properties(jsonrpcstub PROPERTIES FOLDER rpc/jsonrpcstub)

install(TARGETS jsonrpcstub
        DESTINATION bin
        CONFIGURATIONS Release RelWithDebInfo Debug)
install(FILES ${dev_essential_SOURCE_DIR}/scripts/cmake/stub_generation.cmake DESTINATION cmake)
install(DIRECTORY ${dev_essential_SOURCE_DIR}/scripts/cmake/modules DESTINATION cmake)
