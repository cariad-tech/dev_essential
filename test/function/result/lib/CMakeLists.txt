# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS result REQUIRED)

add_executable(error_def_test error_def_test.cpp)
target_link_libraries(error_def_test PRIVATE GTest::gtest_main dev_essential::result)
set_target_properties(error_def_test PROPERTIES FOLDER test/function/a_util/result)
gtest_discover_tests(error_def_test)

add_executable(result_util_test result_util_test.cpp)
target_link_libraries(result_util_test PRIVATE GTest::gtest_main dev_essential::result)
set_target_properties(result_util_test PROPERTIES FOLDER test/function/a_util/result)
gtest_discover_tests(result_util_test)