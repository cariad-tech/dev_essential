# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS csv_reader REQUIRED)

cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/files/test.csv"
           TO_CMAKE_PATH_LIST test_file_path
           NORMALIZE)
add_executable(csv_reader_test src/csv_reader_test.cpp)
target_link_libraries(csv_reader_test PRIVATE GTest::gtest_main dev_essential::csv_reader)
target_compile_definitions(csv_reader_test PRIVATE TEST_FILE_PATH="${test_file_path}")
set_target_properties(csv_reader_test PROPERTIES FOLDER test/function/a_util/csv_reader)
gtest_discover_tests(csv_reader_test)