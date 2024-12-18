# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
if(NOT MSVC)
    find_package(Threads REQUIRED)
endif()

cmake_path(CONVERT "${CMAKE_CURRENT_LIST_DIR}/../../dd/files/"
           TO_CMAKE_PATH_LIST test_files_dir
           NORMALIZE)
add_executable(ddl_infomodel_tests
    ${CMAKE_CURRENT_LIST_DIR}/../../_common/test_oo_ddl.h
    ${CMAKE_CURRENT_LIST_DIR}/../../_common/test_measurement.h
    tester_validationinfo.cpp
    tester_typeinfo.cpp
)
set_target_properties(ddl_infomodel_tests PROPERTIES FOLDER test/function/ddl
                                                     TIMEOUT 120)
target_compile_definitions(ddl_infomodel_tests PRIVATE TEST_FILES_DIR="${test_files_dir}")
target_link_libraries(ddl_infomodel_tests PRIVATE dev_essential::ddl 
                                                  gtest::gtest)
gtest_discover_tests(ddl_infomodel_tests)