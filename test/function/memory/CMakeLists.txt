# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS memory strings REQUIRED)

add_executable(memory_tests memory_test.cpp)
target_link_libraries(memory_tests PRIVATE GTest::gtest_main dev_essential::memory)
set_target_properties(memory_tests PROPERTIES FOLDER test/function/a_util/memory)
gtest_discover_tests(memory_tests)

add_executable(unique_ptr_tests unique_ptr_test.cpp)
target_link_libraries(unique_ptr_tests PRIVATE GTest::gtest_main dev_essential::memory)
set_target_properties(unique_ptr_tests PROPERTIES FOLDER test/function/a_util/memory)
gtest_discover_tests(unique_ptr_tests)

add_executable(make_unique_tests make_unique_test.cpp)
target_link_libraries(make_unique_tests PRIVATE GTest::gtest_main dev_essential::memory)
set_target_properties(make_unique_tests PROPERTIES FOLDER test/function/a_util/memory)
gtest_discover_tests(make_unique_tests)

add_executable(memorybuffer_tests memorybuffer_test.cpp)
target_link_libraries(memorybuffer_tests PRIVATE GTest::gtest_main dev_essential::memory)
set_target_properties(memorybuffer_tests PROPERTIES FOLDER test/function/a_util/memory)
gtest_discover_tests(memorybuffer_tests)

add_executable(shared_ptr_tests shared_ptr_test.cpp shared_ptr_incomplete.cpp)
target_link_libraries(shared_ptr_tests PRIVATE GTest::gtest_main dev_essential::memory)
set_target_properties(shared_ptr_tests PROPERTIES FOLDER test/function/a_util/memory)
gtest_discover_tests(shared_ptr_tests)

add_executable(stack_ptr_tests stack_ptr_test.cpp)
target_link_libraries(stack_ptr_tests PRIVATE GTest::gtest_main dev_essential::memory)
set_target_properties(stack_ptr_tests PROPERTIES FOLDER test/function/a_util/memory)
gtest_discover_tests(stack_ptr_tests)