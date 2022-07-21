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

if(NOT MSVC)
    option(dev_essential_cmake_enable_visibility_hidden
            "If ON, set visibility of target symbols to hidden, otherwise use the default visibility (default: ON)"
           ON)
    if(dev_essential_cmake_enable_visibility_hidden)
        cmake_policy(SET CMP0063 NEW) #See: https://cmake.org/cmake/help/v3.18/policy/CMP0063.html
        set(CMAKE_CXX_VISIBILITY_PRESET hidden)
        set(CMAKE_C_VISIBILITY_PRESET hidden)
        set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)
    endif()
endif()
