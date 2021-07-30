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

find_package(dev_essential COMPONENTS concurrency system REQUIRED)

add_executable(thread_tests thread_test.cpp)
target_link_libraries(thread_tests PRIVATE GTest::gtest_main
                                           dev_essential::concurrency
                                           dev_essential::system)
add_test(thread_tests thread_tests)
set_target_properties(thread_tests PROPERTIES FOLDER test/function/a_util/concurrency)

add_executable(condvar_tests condvar_test.cpp)
target_link_libraries(condvar_tests PRIVATE GTest::gtest_main
                                            dev_essential::concurrency
                                            dev_essential::system)
add_test(condvar_tests condvar_tests)
set_target_properties(condvar_tests PROPERTIES FOLDER test/function/a_util/concurrency)

add_executable(mutex_tests mutex_test.cpp)
target_link_libraries(mutex_tests PRIVATE GTest::gtest_main
                                          dev_essential::concurrency
                                          dev_essential::system)
add_test(mutex_tests mutex_tests)
set_target_properties(mutex_tests PROPERTIES FOLDER test/function/a_util/concurrency)

add_executable(semaphore_tests semaphore_test.cpp)
target_link_libraries(semaphore_tests PRIVATE GTest::gtest_main
                                              dev_essential::concurrency
                                              dev_essential::system)
add_test(semaphore_tests semaphore_tests)
set_target_properties(semaphore_tests PROPERTIES FOLDER test/function/a_util/concurrency)

add_executable(chrono_tests chrono_test.cpp)
target_link_libraries(chrono_tests PRIVATE GTest::gtest_main dev_essential::concurrency)
add_test(chrono_tests chrono_tests)
set_target_properties(chrono_tests PROPERTIES FOLDER test/function/a_util/concurrency)
