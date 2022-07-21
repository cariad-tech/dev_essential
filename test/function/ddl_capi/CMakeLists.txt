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

find_package(dev_essential COMPONENTS ddl_capi REQUIRED)

add_executable(ddl_capi_basic_tests basic_test.cpp test_fixture.h)
target_link_libraries(ddl_capi_basic_tests PRIVATE GTest::gtest_main
                                                   dev_essential::ddl_capi)
set_target_properties(ddl_capi_basic_tests PROPERTIES FOLDER test/function/ddl_capi)
target_compile_definitions(ddl_capi_basic_tests
                           PRIVATE "INPUT_DIR=\"${CMAKE_CURRENT_LIST_DIR}/\""
                                   "OUTPUT_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/\"")
add_custom_command(TARGET ddl_capi_basic_tests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:dev_essential::ddl_capi>
                   $<TARGET_FILE_DIR:ddl_capi_basic_tests>)
add_test(ddl_capi_basic_tests ddl_capi_basic_tests)

add_executable(ddl_capi_negative_tests negative_test.cpp test_fixture.h)
target_link_libraries(ddl_capi_negative_tests PRIVATE GTest::gtest_main
                                                      dev_essential::ddl_capi)
set_target_properties(ddl_capi_negative_tests PROPERTIES FOLDER test/function/ddl_capi)
target_compile_definitions(ddl_capi_negative_tests PRIVATE
                           "INPUT_DIR=\"${CMAKE_CURRENT_LIST_DIR}/\"")
add_custom_command(TARGET ddl_capi_negative_tests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:dev_essential::ddl_capi>
                   $<TARGET_FILE_DIR:ddl_capi_negative_tests>)
add_test(ddl_capi_negative_tests ddl_capi_negative_tests)
