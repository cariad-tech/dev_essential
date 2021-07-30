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
    option(dev_essential_cmake_enable_position_independent_code
           "Enable position independent code for static libraries (default: ON)"
           ON)
    if(dev_essential_cmake_enable_position_independent_code)
        cmake_policy(SET CMP0083 NEW) #See: https://cmake.org/cmake/help/v3.18/policy/CMP0083.html
        include(CheckPIESupported)
        check_pie_supported(OUTPUT_VARIABLE output LANGUAGES CXX)
        set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
        if(NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
            message(INFO "PIE is not supported at link time: ${output}.\n"
                         "PIE link options will not be passed to linker.")
        endif()
    else()
        message(INFO "Position independent code disabled.\n"
                "To activate it use '-Ddev_essential_cmake_enable_position_independent_code=ON'")
    endif()
endif()
