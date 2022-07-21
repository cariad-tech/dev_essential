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

option(dev_essential_cmake_enable_self_contained_header_tests
       "Whether or not to enable tests checking for self contained header files (default: ON)"
       ON)

message(CHECK_START "Check available dev_essential CMake options")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
message(CHECK_START "dev_essential_cmake_enable_self_contained_header_tests")

if(dev_essential_cmake_enable_self_contained_header_tests)
    message(CHECK_PASS "enabled")
    add_subdirectory(self_contained_header)
else()
    message(CHECK_FAIL "disabled")
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
