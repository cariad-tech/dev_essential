# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ddl_dd_create_tests
cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/../files/"
           TO_CMAKE_PATH_LIST test_files_dir
           NORMALIZE)
cmake_path(CONVERT "${CMAKE_CURRENT_BINARY_DIR}/"
           TO_CMAKE_PATH_LIST test_files_write_dir
           NORMALIZE)

add_executable(ddl_dd_create_tests
    ${CMAKE_CURRENT_LIST_DIR}/../../_common/test_oo_ddl.h
    tester_ddstruct.cpp
    tester_ddfile.cpp
    tester_ddstring.cpp
    tester_ddcompare.cpp
    tester_ddtype.cpp
    tester_dddefault.cpp
    tester_ddmerge.cpp
    )
set_source_files_properties(tester_dddefault.cpp tester_ddfile.cpp PROPERTIES COMPILE_DEFINITIONS
    "TEST_FILES_DIR=\"${test_files_dir}\";TEST_FILES_WRITE_DIR=\"${test_files_write_dir}\"")
target_link_libraries(ddl_dd_create_tests PRIVATE dev_essential::ddl
                                                  GTest::gtest_main
                                                  $<$<PLATFORM_ID:Linux>:Threads::Threads>)
set_target_properties(ddl_dd_create_tests PROPERTIES FOLDER test/function/ddl
                                                     TIMEOUT 10)
gtest_discover_tests(ddl_dd_create_tests)

#ddl_dd_create_version_tests
add_library(ddl_version_test_lib
        STATIC
        static_ddl_version_test_lib.h
        static_ddl_version_test_lib.cpp)
set_target_properties(ddl_version_test_lib PROPERTIES FOLDER test/function/ddl/testlib)
target_link_libraries(ddl_version_test_lib PRIVATE dev_essential::ddl)

add_executable(ddl_dd_create_version_tests tester_ddl_version.cpp)
target_link_libraries(ddl_dd_create_version_tests PRIVATE ddl_version_test_lib
                                                          dev_essential::ddl
                                                          GTest::gtest_main
                                                          $<$<PLATFORM_ID:Linux>:Threads::Threads>)
set_target_properties(ddl_dd_create_version_tests PROPERTIES FOLDER test/function/ddl)
gtest_discover_tests(ddl_dd_create_version_tests)

#ddl_dd_datastructures_tests
add_executable(ddl_dd_datastructures_tests tester_dd_datastructures.cpp)
set_target_properties(ddl_dd_datastructures_tests PROPERTIES FOLDER test/function/ddl
                                                             TIMEOUT 5)
target_link_libraries(ddl_dd_datastructures_tests PRIVATE dev_essential::ddl
                                                          GTest::gtest_main
                                                          $<$<PLATFORM_ID:Linux>:Threads::Threads>)
gtest_discover_tests(ddl_dd_datastructures_tests)
