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

find_package(dev_essential COMPONENTS regex REQUIRED)

add_executable(regex_tests regex_test.cpp)

# Link test executable against gtest & gtest_main (also gets their include paths)
target_link_libraries(regex_tests PRIVATE GTest::gtest_main dev_essential::regex)

add_test(regex_tests regex_tests)
set_target_properties(regex_tests PROPERTIES FOLDER test/function/a_util/regex)
