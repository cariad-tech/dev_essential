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

find_package(dev_essential COMPONENTS xml filesystem REQUIRED)

add_executable(xml_tests src/xml_test.cpp)

# Link test executable against gtest & gtest_main (also gets their include paths)
target_link_libraries(xml_tests PRIVATE GTest::gtest_main dev_essential::xml)

add_test(NAME xml_tests COMMAND ${CMAKE_CURRENT_BINARY_DIR}/xml_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(xml_tests PROPERTIES FOLDER test/function/a_util/xml)
