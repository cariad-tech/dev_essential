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

add_subdirectory(../../extern/3rdparty/cpp-httplib ${CMAKE_BINARY_DIR}/extern/3rdparty/cpp-httplib)
add_subdirectory(../../extern/3rdparty/jsoncpp ${CMAKE_BINARY_DIR}/extern/3rdparty/jsoncpp)
add_subdirectory(../../extern/3rdparty/libjson-rpc-cpp ${CMAKE_BINARY_DIR}/extern/3rdparty/libjson-rpc-cpp)

set(PROJECT_NAME pkg_rpc)
add_library(${PROJECT_NAME} STATIC ../../include/rpc/rpc.h
                                   ../../include/rpc/rpc_server.h
                                   ../../include/rpc/json_rpc.h
                                   ../../include/rpc/rpc_object_registry.h
                                   ../../include/rpc/detail/json_rpc_impl.h
                                   ../../include/rpc/http/threaded_http_server.h
                                   ../../include/rpc/http/http_rpc_server.h
                                   ../../include/rpc/http/json_http_rpc.h
                                   http_rpc_server.cpp
                                   json_http_rpc.cpp
                                   threaded_http_server.cpp
                                   json_rpc.cpp
                                   rpc_object_registry.cpp
                                   url.h
                                   url.cpp
                                   $<TARGET_OBJECTS:jsoncpp>
                                   $<TARGET_OBJECTS:libjson-rpc-cpp>)
get_target_property(JSONCPP_INCLUDE_DIRECTORIES jsoncpp INCLUDE_DIRECTORIES)
get_target_property(LIBJSON_RPC_CPP_INCLUDE_DIRECTORIES libjson-rpc-cpp INCLUDE_DIRECTORIES)
# dynamic_cast in jsonrpccpp/server/abstractserver.h
target_compile_options(${PROJECT_NAME} PRIVATE $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<COMPILE_LANGUAGE:CXX>>:-frtti>
                                               $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<COMPILE_LANGUAGE:CXX>>:/GR>)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
                           $<BUILD_INTERFACE:${JSONCPP_INCLUDE_DIRECTORIES}>
                           $<BUILD_INTERFACE:${LIBJSON_RPC_CPP_INCLUDE_DIRECTORIES}>)
target_link_libraries(${PROJECT_NAME} PUBLIC result
                                      PRIVATE regex concurrency
                                              $<$<PLATFORM_ID:Windows>:ws2_32>
                                              $<BUILD_INTERFACE:httplib>)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER rpc/pkg_rpc
                                                 OUTPUT_NAME pkg_rpc)
install(TARGETS ${PROJECT_NAME})
install(DIRECTORY ${dev_essential_SOURCE_DIR}/include/rpc DESTINATION include)

add_subdirectory(stubgenerator)
