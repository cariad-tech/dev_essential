# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS preprocessor REQUIRED)

# current_function test
add_executable(current_function_tests current_function_tests.cpp)
target_link_libraries(current_function_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
set_target_properties(current_function_tests PROPERTIES FOLDER test/function/a_util/preprocessor)
gtest_discover_tests(current_function_tests)

# deprecated macro tests
add_executable(deprecated_tests deprecated_tests.cpp)
target_link_libraries(deprecated_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
set_target_properties(deprecated_tests PROPERTIES FOLDER test/function/a_util/preprocessor)
gtest_discover_tests(deprecated_tests)

# dll_export tests
add_executable(dll_export_tests dll_export_tests.cpp)
target_link_libraries(dll_export_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
set_target_properties(dll_export_tests PROPERTIES FOLDER test/function/a_util/preprocessor)
target_compile_definitions(dll_export_tests PRIVATE A_UTIL_DO_EXPORT)
gtest_discover_tests(dll_export_tests)

# dll_import tests
add_executable(dll_import_tests dll_import_tests.cpp)
target_link_libraries(dll_import_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
set_target_properties(dll_import_tests PROPERTIES FOLDER test/function/a_util/preprocessor)
gtest_discover_tests(dll_import_tests)

# to_string test
add_executable(to_string_tests to_string_tests.cpp)
target_link_libraries(to_string_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
set_target_properties(to_string_tests PROPERTIES FOLDER test/function/a_util/preprocessor)
gtest_discover_tests(to_string_tests)