# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS ddl_capi REQUIRED)

cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/files/"
           TO_CMAKE_PATH_LIST test_files_dir
           NORMALIZE)
cmake_path(CONVERT "${CMAKE_CURRENT_BINARY_DIR}/"
           TO_CMAKE_PATH_LIST test_files_write_dir
           NORMALIZE)

# basic_tests
add_executable(ddl_capi_basic_tests basic_test.cpp test_fixture.h)
target_link_libraries(ddl_capi_basic_tests PRIVATE GTest::gtest_main
                                                   dev_essential::ddl_capi)
set_target_properties(ddl_capi_basic_tests PROPERTIES FOLDER test/function/ddl_capi)
target_compile_definitions(ddl_capi_basic_tests
                           PRIVATE TEST_FILES_DIR="${test_files_dir}"
                                   TEST_FILES_WRITE_DIR="${test_files_write_dir}")
add_custom_command(TARGET ddl_capi_basic_tests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:dev_essential::ddl_capi>
                   $<TARGET_FILE_DIR:ddl_capi_basic_tests>)
gtest_discover_tests(ddl_capi_basic_tests)

# negative_tests
add_executable(ddl_capi_negative_tests negative_test.cpp test_fixture.h)
target_link_libraries(ddl_capi_negative_tests PRIVATE GTest::gtest_main
                                                      dev_essential::ddl_capi)
set_target_properties(ddl_capi_negative_tests PROPERTIES FOLDER test/function/ddl_capi)
target_compile_definitions(ddl_capi_negative_tests PRIVATE TEST_FILES_DIR="${test_files_dir}")
add_custom_command(TARGET ddl_capi_negative_tests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:dev_essential::ddl_capi>
                   $<TARGET_FILE_DIR:ddl_capi_negative_tests>)
gtest_discover_tests(ddl_capi_negative_tests)
