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

set(TEST_NAME codec)
add_executable(ddl_${TEST_NAME}_tests tester_${TEST_NAME}.cpp)

set_target_properties(ddl_${TEST_NAME}_tests PROPERTIES FOLDER test/function/ddl)
set_target_properties(ddl_${TEST_NAME}_tests  PROPERTIES TIMEOUT 300)

target_compile_definitions(ddl_${TEST_NAME}_tests
                           PRIVATE TEST_FILES_DIR="${CMAKE_CURRENT_LIST_DIR}/../files"
                                   $<$<CONFIG:Debug>:BUILD_TYPE="DEBUG">
                                   $<$<NOT:$<CONFIG:Debug>>:BUILD_TYPE="RELEASE">)

target_link_libraries(ddl_${TEST_NAME}_tests PRIVATE
    dev_essential::ddl
    GTest::gtest_main
    $<$<PLATFORM_ID:Linux>:Threads::Threads>
)
add_test(ddl_${TEST_NAME}_tests ddl_${TEST_NAME}_tests)


set(TEST_NAME mediacoder_regression)
add_executable(ddl_${TEST_NAME}_tests tester_${TEST_NAME}.cpp)
add_test(NAME ddl_${TEST_NAME}_tests
         COMMAND ddl_${TEST_NAME}_tests
         WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
         )

target_compile_definitions(ddl_${TEST_NAME}_tests
                           PRIVATE TEST_FILES_DIR="${CMAKE_CURRENT_LIST_DIR}/../files"
                                   TEST_FILES_WRITE_DIR="${CMAKE_CURRENT_BINARY_DIR}/")

set_target_properties(ddl_${TEST_NAME}_tests PROPERTIES FOLDER test/function/ddl)
set_target_properties(ddl_${TEST_NAME}_tests PROPERTIES TIMEOUT 300)

target_link_libraries(ddl_${TEST_NAME}_tests PRIVATE
    ddl_test_compat
    dev_essential::ddl
    GTest::gtest_main
    $<$<PLATFORM_ID:Linux>:Threads::Threads>
)
