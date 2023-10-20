# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS xml filesystem REQUIRED)

cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/files/"
           TO_CMAKE_PATH_LIST test_files_dir
           NORMALIZE)
cmake_path(CONVERT "${CMAKE_CURRENT_BINARY_DIR}/"
           TO_CMAKE_PATH_LIST test_files_write_dir
           NORMALIZE)

add_executable(xml_tests src/xml_test.cpp
                         src/xml_test_sort.cpp)
target_link_libraries(xml_tests PRIVATE GTest::gtest_main
                                        dev_essential::xml
                                        dev_essential::filesystem)
target_compile_definitions(xml_tests PRIVATE TEST_FILES_DIR="${test_files_dir}"
                                             TEST_FILES_WRITE_DIR="${test_files_write_dir}")
set_target_properties(xml_tests PROPERTIES FOLDER test/function/a_util/xml)
gtest_discover_tests(xml_tests)