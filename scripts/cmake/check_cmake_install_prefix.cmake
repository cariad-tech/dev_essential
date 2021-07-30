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

## This must always be the first check when running cmake
set(CURRENT_OPTION "dev_essential_cmake_enable_warn_on_default_cmake_install_prefix")
option(${CURRENT_OPTION} "Warn if 'CMAKE_INSTALL_PREFIX is set to default (default: ON)" ON)

if(${CURRENT_OPTION})
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR
       ";${util_cmake_CMAKE_DEFAULT_INSTALL_PREFIX};" STREQUAL ";${CMAKE_INSTALL_PREFIX};")
        message(WARNING "'CMAKE_INSTALL_PREFIX' is currently set to the default value. Suppress \
this check by disabling '${CURRENT_OPTION}'")

        ## Enable warning for more than just the first run of cmake
        set(util_cmake_CMAKE_DEFAULT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE INTERNAL "")
    else()
        unset(util_cmake_CMAKE_DEFAULT_INSTALL_PREFIX)
    endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR
          ";${util_cmake_CMAKE_DEFAULT_INSTALL_PREFIX};" STREQUAL ";${CMAKE_INSTALL_PREFIX};")
endif(${CURRENT_OPTION})

unset(CURRENT_OPTION)
