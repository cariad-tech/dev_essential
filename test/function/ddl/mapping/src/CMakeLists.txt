# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
if(NOT MSVC)
    find_package(Threads REQUIRED)
endif()

cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/../files/"
           TO_CMAKE_PATH_LIST test_files_dir
           NORMALIZE)

cmake_path(CONVERT "${CMAKE_CURRENT_BINARY_DIR}/"
           TO_CMAKE_PATH_LIST test_files_write_dir
           NORMALIZE)

add_executable(ddl_mapping_tests tester_mapping.cpp)
set_target_properties(ddl_mapping_tests PROPERTIES FOLDER test/function/ddl
                                                   TIMEOUT 120)
target_compile_definitions(ddl_mapping_tests PRIVATE TEST_FILES_DIR="${test_files_dir}"
                                                     TEST_FILES_WRITE_DIR="${test_files_write_dir}")
target_link_libraries(ddl_mapping_tests PRIVATE ddl_test_compat
                                                dev_essential::ddl
                                                gtest::gtest)
gtest_discover_tests(ddl_mapping_tests)