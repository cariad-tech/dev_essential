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

find_package(dev_essential COMPONENTS datetime system REQUIRED)

add_executable(datetime_tests datetime_test.cpp)
target_link_libraries(datetime_tests PRIVATE GTest::gtest_main
                                             dev_essential::datetime
                                             dev_essential::system)
add_test(datetime_tests datetime_tests)
set_target_properties(datetime_tests PROPERTIES FOLDER test/function/a_util/datetime)
