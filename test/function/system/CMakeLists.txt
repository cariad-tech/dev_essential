# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS system filesystem REQUIRED)

#system_tests
add_executable(system_tests system_test.cpp)
target_link_libraries(system_tests PRIVATE GTest::gtest_main
                                           dev_essential::system
                                           dev_essential::filesystem) #Path
set_target_properties(system_tests PROPERTIES FOLDER test/function/a_util/system)
target_compile_definitions(system_tests PRIVATE TEST_EXE_FILE=\"$<TARGET_FILE:system_tests>\")
gtest_discover_tests(system_tests)

#timer tests
add_executable(timer_tests timer_test.cpp)
target_link_libraries(timer_tests PRIVATE GTest::gtest_main dev_essential::system)
set_target_properties(timer_tests PROPERTIES FOLDER test/function/a_util/system)
gtest_discover_tests(timer_tests)

#uuid tests
add_executable(uuid_tests uuid_test.cpp)
target_link_libraries(uuid_tests PRIVATE GTest::gtest_main
                                         dev_essential::system
                                         $<$<BOOL:${QNXNTO}>:socket>)
set_target_properties(uuid_tests PROPERTIES FOLDER test/function/a_util/system)
gtest_discover_tests(uuid_tests)

#address_info_tests
add_library(test_static_library STATIC test_static_library.cpp)
target_link_libraries(test_static_library PRIVATE dev_essential::system)
set_target_properties(test_static_library PROPERTIES FOLDER test/function/a_util/system/dependencies)

add_library(test_shared_library SHARED test_shared_library.cpp)
target_compile_definitions(test_shared_library PRIVATE A_UTIL_DO_EXPORT)
target_link_libraries(test_shared_library PRIVATE dev_essential::system)
set_target_properties(test_shared_library PROPERTIES FOLDER test/function/a_util/system/dependencies)

add_executable(address_info_tests address_info_test.cpp)
target_link_libraries(address_info_tests PRIVATE GTest::gtest_main
                                                 dev_essential::system
                                                 test_static_library)
set_target_properties(address_info_tests PROPERTIES FOLDER test/function/a_util/system)
add_dependencies(address_info_tests test_shared_library)
target_compile_definitions(address_info_tests PRIVATE
    SHARED_LIB_PATH="$<TARGET_FILE:test_shared_library>"
    TEST_EXE_PATH="$<TARGET_FILE:address_info_tests>"
)
gtest_discover_tests(address_info_tests)