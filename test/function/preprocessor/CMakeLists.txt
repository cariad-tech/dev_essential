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

find_package(dev_essential COMPONENTS preprocessor REQUIRED)

# current_function test
add_executable(current_function_tests current_function_tests.cpp)
target_link_libraries(current_function_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
add_test(current_function_tests current_function_tests)
set_target_properties(current_function_tests PROPERTIES FOLDER test/function/a_util/preprocessor)

# dll_export test
add_executable(dll_export_tests dll_export_tests.cpp)
target_link_libraries(dll_export_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
add_test(dll_export_tests dll_export_tests)
set_target_properties(dll_export_tests PROPERTIES FOLDER test/function/a_util/preprocessor)

# numeric_to_string test
add_executable(to_string_tests to_string_tests.cpp)
target_link_libraries(to_string_tests PRIVATE GTest::gtest_main dev_essential::preprocessor)
add_test(to_string_tests to_string_tests)
set_target_properties(to_string_tests PROPERTIES FOLDER test/function/a_util/preprocessor)
