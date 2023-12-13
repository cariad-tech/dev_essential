# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS strings memory REQUIRED)

##strings_tests
add_executable(strings_tests strings_test.cpp)
target_link_libraries(strings_tests PRIVATE GTest::gtest_main
                                            dev_essential::strings
                                            dev_essential::memory) #MemoryBuffer
set_target_properties(strings_tests PROPERTIES FOLDER test/function/a_util/strings)
gtest_discover_tests(strings_tests)

##strings_convert_tests
add_executable(strings_convert_tests strings_convert_test.cpp)
target_link_libraries(strings_convert_tests PRIVATE GTest::gtest_main dev_essential::strings)
set_target_properties(strings_convert_tests PROPERTIES FOLDER test/function/a_util/strings)
gtest_discover_tests(strings_convert_tests)

##strings_format_tests
add_executable(strings_format_tests strings_format_test.cpp)
target_link_libraries(strings_format_tests PRIVATE GTest::gtest_main dev_essential::strings)
set_target_properties(strings_format_tests PROPERTIES FOLDER test/function/a_util/strings)
gtest_discover_tests(strings_format_tests)

##strings_numeric_traits_tests
add_executable(strings_numeric_traits_tests strings_numeric_traits_test.cpp)
target_link_libraries(strings_numeric_traits_tests PRIVATE GTest::gtest_main dev_essential::strings)
set_target_properties(strings_numeric_traits_tests PROPERTIES FOLDER test/function/a_util/strings)
gtest_discover_tests(strings_numeric_traits_tests)

##unicode_tests
add_executable(unicode_tests unicode_test.cpp)
target_link_libraries(unicode_tests PRIVATE GTest::gtest_main dev_essential::strings)
set_target_properties(unicode_tests PROPERTIES FOLDER test/function/a_util/strings)
gtest_discover_tests(unicode_tests)