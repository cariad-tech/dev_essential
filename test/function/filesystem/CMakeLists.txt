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

find_package(dev_essential COMPONENTS filesystem REQUIRED)

add_executable(filesystem_tests filesystem_test.cpp)
target_link_libraries(filesystem_tests PRIVATE GTest::gtest_main dev_essential::filesystem)
add_test(filesystem_tests filesystem_tests)
set_target_properties(filesystem_tests PROPERTIES FOLDER test/function/a_util/filesystem)

add_executable(path_tests path_test.cpp)
target_link_libraries(path_tests PRIVATE GTest::gtest_main dev_essential::filesystem)
add_test(path_tests path_tests)
set_target_properties(path_tests PROPERTIES FOLDER test/function/a_util/filesystem)
