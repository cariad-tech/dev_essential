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

## Check build type and terminate cmake if none is given
if(UNIX)
    if(NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";Debug;" AND
       NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";Release;" AND
       NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";RelWithDebInfo;" AND
       NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";MinSizeRel;")
        message(FATAL_ERROR "'CMAKE_BUILD_TYPE' is either empty or set to an unknown build type")
    endif(NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";Debug;" AND
          NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";Release;" AND
          NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";RelWithDebInfo;" AND
          NOT ";${CMAKE_BUILD_TYPE};" STREQUAL ";MinSizeRel;")
endif(UNIX)
