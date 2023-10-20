# Copyright @ 2022 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

option(dev_essential_cmake_enable_third_party_tests
       "Enable tests of 3rdparty dependencies (default: OFF)"
       OFF)

if (dev_essential_cmake_enable_third_party_tests)
    add_subdirectory(3rdparty)
endif()