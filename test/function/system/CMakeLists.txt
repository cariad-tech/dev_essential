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

find_package(dev_essential COMPONENTS system filesystem REQUIRED)

##system_tests
add_executable(system_tests system_test.cpp)
target_link_libraries(system_tests PRIVATE GTest::gtest_main
                                           dev_essential::system
                                           dev_essential::filesystem) #Path
add_test(system_tests system_tests)
set_target_properties(system_tests PROPERTIES FOLDER test/function/a_util/system)
target_compile_definitions(system_tests PRIVATE TEST_EXE_FILE=\"$<TARGET_FILE:system_tests>\")

add_executable(timer_tests timer_test.cpp)
target_link_libraries(timer_tests PRIVATE GTest::gtest_main dev_essential::system)
add_test(timer_tests timer_tests)
set_target_properties(timer_tests PROPERTIES FOLDER test/function/a_util/system)

add_executable(uuid_tests uuid_test.cpp)
target_link_libraries(uuid_tests PRIVATE GTest::gtest_main dev_essential::system)
if(QNXNTO)
    target_link_libraries(uuid_tests PUBLIC socket)
endif()
add_test(uuid_tests uuid_tests)
set_target_properties(uuid_tests PROPERTIES FOLDER test/function/a_util/system)
