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

find_package(dev_essential COMPONENTS base process REQUIRED)

##process_tests
add_executable(process_tests process_execute_test.cpp process_env_test.cpp)
target_link_libraries(process_tests PRIVATE GTest::gtest_main dev_essential::process)
add_test(process_tests process_tests)
set_target_properties(process_tests PROPERTIES FOLDER test/function/a_util/process)
