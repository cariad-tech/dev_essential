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

find_package(dev_essential COMPONENTS result REQUIRED)

add_executable(error_def_test error_def_test.cpp)
target_link_libraries(error_def_test PRIVATE GTest::gtest_main dev_essential::result)
add_test(error_def_test error_def_test)
set_target_properties(error_def_test PROPERTIES FOLDER test/function/a_util/result)

add_executable(result_util_test result_util_test.cpp)
target_link_libraries(result_util_test PRIVATE GTest::gtest_main dev_essential::result)
add_test(result_util_test result_util_test)
set_target_properties(result_util_test PROPERTIES FOLDER test/function/a_util/result)
