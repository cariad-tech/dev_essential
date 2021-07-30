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

find_package(dev_essential COMPONENTS csv_reader REQUIRED)
add_executable(csv_reader_test src/csv_reader_test.cpp)

target_link_libraries(csv_reader_test PRIVATE GTest::gtest_main dev_essential::csv_reader)
add_test(NAME csv_reader_test
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/csv_reader_test
         WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(csv_reader_test PROPERTIES FOLDER test/function/a_util/csv_reader)
