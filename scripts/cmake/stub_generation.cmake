# Copyright @ 2023 VW Group. All rights reserved.
#
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

macro(jsonrpc_generate_client_stub JSON_RPC_DEFINITION_FILE CLIENT_CLASS_NAME CLIENT_FILE_NAME)
    message(STATUS "will generate clientstub to ${CLIENT_FILE_NAME}")
    add_custom_command(OUTPUT ${CLIENT_FILE_NAME}
                       COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:dev_essential::jsonrpcstub> ${JSON_RPC_DEFINITION_FILE} --cpp-client=${CLIENT_CLASS_NAME} --cpp-client-file=${CLIENT_FILE_NAME}
                       DEPENDS ${JSON_RPC_DEFINITION_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "generating json rpc client stub ${CLIENT_FILE_NAME}")
endmacro(jsonrpc_generate_client_stub)

macro(jsonrpc_generate_server_stub JSON_RPC_DEFINITION_FILE SERVER_CLASS_NAME SERVER_FILE_NAME)
    message(STATUS "will generate serverstub to ${SERVER_FILE_NAME}")
    add_custom_command(OUTPUT ${SERVER_FILE_NAME}
                       COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:dev_essential::jsonrpcstub> ${JSON_RPC_DEFINITION_FILE} --cpp-server=${SERVER_CLASS_NAME} --cpp-server-file=${SERVER_FILE_NAME}
                       DEPENDS ${JSON_RPC_DEFINITION_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "generating json rpc server stub ${SERVER_FILE_NAME}")
endmacro(jsonrpc_generate_server_stub)
