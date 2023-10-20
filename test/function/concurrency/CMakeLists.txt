# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS concurrency system REQUIRED)

#thread_tests
add_executable(thread_tests thread_test.cpp)
target_link_libraries(thread_tests PRIVATE GTest::gtest_main
                                           dev_essential::concurrency
                                           dev_essential::system)
set_target_properties(thread_tests PROPERTIES FOLDER test/function/a_util/concurrency)
gtest_discover_tests(thread_tests)

#condvar_tests
add_executable(condvar_tests condvar_test.cpp)
target_link_libraries(condvar_tests PRIVATE GTest::gtest_main
                                            dev_essential::concurrency
                                            dev_essential::system)
set_target_properties(condvar_tests PROPERTIES FOLDER test/function/a_util/concurrency)
gtest_discover_tests(condvar_tests)

#mutex_tests
add_executable(mutex_tests mutex_test.cpp)
target_link_libraries(mutex_tests PRIVATE GTest::gtest_main
                                          dev_essential::concurrency
                                          dev_essential::system)
set_target_properties(mutex_tests PROPERTIES FOLDER test/function/a_util/concurrency)
gtest_discover_tests(mutex_tests)

#semaphore_tests
add_executable(semaphore_tests semaphore_test.cpp)
target_link_libraries(semaphore_tests PRIVATE GTest::gtest_main
                                              dev_essential::concurrency
                                              dev_essential::system)
set_target_properties(semaphore_tests PROPERTIES FOLDER test/function/a_util/concurrency)
gtest_discover_tests(semaphore_tests PROPERTIES TIMEOUT 1) # for each discovered test ...

#chrono_tests
add_executable(chrono_tests chrono_test.cpp)
target_link_libraries(chrono_tests PRIVATE GTest::gtest_main dev_essential::concurrency)
set_target_properties(chrono_tests PROPERTIES FOLDER test/function/a_util/concurrency)
gtest_discover_tests(chrono_tests)
