/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    errors.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "errors.h"
#include "exception.h"

using namespace jsonrpc;

const int Errors::ERROR_RPC_JSON_PARSE_ERROR = -32700;
const int Errors::ERROR_RPC_METHOD_NOT_FOUND = -32601;
const int Errors::ERROR_RPC_INVALID_REQUEST = -32600;
const int Errors::ERROR_RPC_INVALID_PARAMS = -32602;
const int Errors::ERROR_RPC_INTERNAL_ERROR = -32603;

const int Errors::ERROR_SERVER_PROCEDURE_IS_METHOD = -32604;
const int Errors::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION = -32605;
const int Errors::ERROR_SERVER_PROCEDURE_POINTER_IS_NULL = -32606;
const int Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND = -32000;
const int Errors::ERROR_SERVER_CONNECTOR = -32002;
const int Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX = -32007;

const int Errors::ERROR_CLIENT_CONNECTOR = -32003;
const int Errors::ERROR_CLIENT_INVALID_RESPONSE = -32001;

std::string Errors::GetErrorMessage(int errorCode) {
  // Official Errors
  const static std::map<int, const std::string> possibleErrors {
  { ERROR_RPC_INVALID_REQUEST, "INVALID_JSON_REQUEST: The JSON "
                                              "sent is not a valid JSON-RPC "
                                              "Request object"},
  { ERROR_RPC_METHOD_NOT_FOUND, "METHOD_NOT_FOUND: The method "
                                               "being requested is not "
                                               "available on this server"},
  { ERROR_RPC_INVALID_PARAMS, "INVALID_PARAMS: Invalid method "
                                             "parameters (invalid name and/or "
                                             "type) recognised"},
  { ERROR_RPC_JSON_PARSE_ERROR,
      "JSON_PARSE_ERROR: The JSON-Object is not JSON-Valid"},
  { ERROR_RPC_INTERNAL_ERROR, "INTERNAL_ERROR: "},

  { ERROR_SERVER_PROCEDURE_IS_METHOD,
      "PROCEDURE_IS_METHOD: The requested notification is declared as a method"},
  { ERROR_SERVER_PROCEDURE_IS_NOTIFICATION,
      "PROCEDURE_IS_NOTIFICATION: The requested method is declared as "
      "notification"},
  { ERROR_SERVER_PROCEDURE_POINTER_IS_NULL,
      "PROCEDURE_POINTER_IS_NULL: Server has no function Reference registered"},
  { ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND,
      "Configuration file was not found"},

  { ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX,
      "Procedure specification mallformed"},

  { ERROR_CLIENT_INVALID_RESPONSE, "The response is invalid"},
  { ERROR_CLIENT_CONNECTOR, "Client connector error"},
  { ERROR_SERVER_CONNECTOR, "Server connector error"},
  };


  if (possibleErrors.find(errorCode) == possibleErrors.end()) {
    return "";
  }

  return possibleErrors.at(errorCode);
}
