# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS filesystem REQUIRED)

add_executable(filesystem_tests filesystem_test.cpp)
target_link_libraries(filesystem_tests PRIVATE GTest::gtest_main dev_essential::filesystem)
set_target_properties(filesystem_tests PROPERTIES FOLDER test/function/a_util/filesystem)
gtest_discover_tests(filesystem_tests)

add_executable(path_tests path_test.cpp)
target_link_libraries(path_tests PRIVATE GTest::gtest_main dev_essential::filesystem)
set_target_properties(path_tests PROPERTIES FOLDER test/function/a_util/filesystem)
gtest_discover_tests(path_tests)
