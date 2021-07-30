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

# For cross compiling we need the stubgenerator tool of the build host, otherwise the executable
# from pkg_rpc is used which is for the build target and cannot be run on the host
if(NOT TARGET jsonrpcstub)
    find_program(JSONRPCSTUB_EXECUTABLE NAMES jsonrpcstub)
    if(JSONRPCSTUB_EXECUTABLE)
        execute_process(
            COMMAND "${JSONRPCSTUB_EXECUTABLE}" --version
            OUTPUT_VARIABLE JSONRPCSTUB_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE _Jsonrpcstub_version_result)
        if(_Jsonrpcstub_version_result)
            message(FATAL_ERROR "Unable to determine version for ${JSONRPCSTUB_EXECUTABLE}: ${_Jsonrpcstub_version_result}")
        endif()
        message(STATUS "Found jsonrpcstub generator: ${JSONRPCSTUB_EXECUTABLE} (found version: \"${JSONRPCSTUB_VERSION}\")")
    else()
        message(FATAL_ERROR "Unable to find a jsonrpcstub generator executable. Mandatory client and server stubs cannot be generated.")
    endif()

    add_executable(jsonrpcstub IMPORTED)
    set_target_properties(jsonrpcstub PROPERTIES IMPORTED_LOCATION "${JSONRPCSTUB_EXECUTABLE}")
endif()
