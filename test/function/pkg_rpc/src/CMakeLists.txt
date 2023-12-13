# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS pkg_rpc REQUIRED)

if(CMAKE_CROSSCOMPILING)
    find_package(JsonStubGenerator REQUIRED)
endif()

jsonrpc_generate_client_stub(${CMAKE_CURRENT_SOURCE_DIR}/test.json          #specfile
                             rpc_stubs::cTestClientStub                     #--cpp-client
                             ${CMAKE_CURRENT_BINARY_DIR}/testclientstub.h   #--cpp-client-file
                             )
jsonrpc_generate_server_stub(${CMAKE_CURRENT_SOURCE_DIR}/test.json          #specfile
                             rpc_stubs::cTestServerStub                     #--cpp-server
                             ${CMAKE_CURRENT_BINARY_DIR}/testserverstub.h   #--cpp-server-file
                             )

add_executable(pkg_rpc_test tester_pkg_rpc.cpp
                            test.json
                            ${CMAKE_CURRENT_BINARY_DIR}/testclientstub.h
                            ${CMAKE_CURRENT_BINARY_DIR}/testserverstub.h)
set_target_properties(pkg_rpc_test PROPERTIES FOLDER test/function/rpc)
target_include_directories(pkg_rpc_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(pkg_rpc_test PRIVATE GTest::gtest_main dev_essential::pkg_rpc
                                           $<$<BOOL:${QNXNTO}>:socket>)
gtest_discover_tests(pkg_rpc_test)

# intermediate target in case only the client/server stub header need to be generated
add_custom_target(pkg_rpc_test_generate_rpc_stubs
                  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/testclientstub.h
                          ${CMAKE_CURRENT_BINARY_DIR}/testserverstub.h)
set_target_properties(pkg_rpc_test_generate_rpc_stubs PROPERTIES FOLDER test/sca/rpc)

# jsonrpcstubgenerator tests
## need a dedicated directory to check whether files are generated
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test_files_write_dir")
add_executable(jsonrpcstub_tests tester_jsonrpcstub.cpp)
target_link_libraries(jsonrpcstub_tests PRIVATE GTest::gtest_main dev_essential::filesystem)
target_compile_definitions(jsonrpcstub_tests
                           PRIVATE STUBGENERATOR_EXE="$<TARGET_FILE:jsonrpcstub>"
                                   SPECIFICATION_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test.json"
                                   TEST_FILES_WRITE_DIR="${CMAKE_CURRENT_BINARY_DIR}/test_files_write_dir/"
                           )
set_target_properties(jsonrpcstub_tests PROPERTIES FOLDER test/function/rpc)
if(dev_essential_cmake_enable_integrated_tests)
    add_dependencies(jsonrpcstub_tests jsonrpcstub)
endif()
gtest_discover_tests(jsonrpcstub_tests)
