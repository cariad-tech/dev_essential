# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS regex REQUIRED)

add_executable(regex_tests regex_test.cpp)
target_link_libraries(regex_tests PRIVATE gtest::gtest dev_essential::regex)
set_target_properties(regex_tests PROPERTIES FOLDER test/function/a_util/regex)
gtest_discover_tests(regex_tests)