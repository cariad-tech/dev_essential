# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

find_package(dev_essential COMPONENTS logging REQUIRED)

##logging_tests
add_executable(logging_tests logging_test.cpp)
target_link_libraries(logging_tests PRIVATE GTest::gtest_main dev_essential::logging)
set_target_properties(logging_tests PROPERTIES FOLDER test/function/a_util/logging)
gtest_discover_tests(logging_tests)
