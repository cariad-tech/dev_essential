# Copyright @ 2021 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

if(NOT TARGET jsonrpcstub)
    if(CMAKE_CROSSCOMPILING)
        find_package(JsonStubGenerator REQUIRED)
    else()
        get_filename_component(_jsonrpcstub_path "${dev_essential_DIR}/../../../bin/jsonrpcstub"
                               ABSOLUTE)
        add_executable(jsonrpcstub IMPORTED GLOBAL)
        set_target_properties(jsonrpcstub PROPERTIES IMPORTED_LOCATION "${_jsonrpcstub_path}")
        message(STATUS "Imported jsonrpcstub generator: ${_jsonrpcstub_path}")
    endif()
endif(NOT TARGET jsonrpcstub)

macro(jsonrpc_generate_client_stub JSON_RPC_DEFINITION_FILE CLIENT_CLASS_NAME CLIENT_FILE_NAME)
    message(STATUS "will generate clientstub to ${CLIENT_FILE_NAME}")
    add_custom_command(OUTPUT ${CLIENT_FILE_NAME}
                       COMMAND jsonrpcstub ${JSON_RPC_DEFINITION_FILE} --cpp-client=${CLIENT_CLASS_NAME} --cpp-client-file=${CLIENT_FILE_NAME}
                       DEPENDS ${JSON_RPC_DEFINITION_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "generating json rpc client stub ${CLIENT_FILE_NAME}")
endmacro(jsonrpc_generate_client_stub)

macro(jsonrpc_generate_server_stub JSON_RPC_DEFINITION_FILE SERVER_CLASS_NAME SERVER_FILE_NAME)
    message(STATUS "will generate serverstub to ${SERVER_FILE_NAME}")
    add_custom_command(OUTPUT ${SERVER_FILE_NAME}
                       COMMAND jsonrpcstub ${JSON_RPC_DEFINITION_FILE} --cpp-server=${SERVER_CLASS_NAME} --cpp-server-file=${SERVER_FILE_NAME}
                       DEPENDS ${JSON_RPC_DEFINITION_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "generating json rpc server stub ${SERVER_FILE_NAME}")
endmacro(jsonrpc_generate_server_stub)
