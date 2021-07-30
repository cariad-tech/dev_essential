/**
 * @file
 * RPC Protocol declaration.
 *
 * @copyright
 * @verbatim
Copyright @ 2021 VW Group. All rights reserved.

    This Source Code Form is subject to the terms of the Mozilla
    Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

If it is not possible or desirable to put the notice in a particular file, then
You may include the notice in a location (such as a LICENSE file in a
relevant directory) where a recipient would be likely to look for such a notice.

You may add additional accurate notices of copyright ownership.
@endverbatim
 */

#ifndef PKG_RPC_JSON_HTTP_H_INCLUDED
#define PKG_RPC_JSON_HTTP_H_INCLUDED

#include "rpc/http/http_rpc_server.h"

#include <jsonrpccpp/client/iclientconnector.h>

namespace rpc {
namespace http {

/**
 * The JSON RPC server class
 */
class cJSONRPCServer : public detail::cRPCServer {
public:
    /// CTOR, setting the content type for the RPC server to "application/json"
    cJSONRPCServer();
};

/**
 * Connector that sends RPC messages via HTTP
 */
class cJSONClientConnector : public jsonrpc::IClientConnector {
public:
    /**
     * Constructor
     * @param[in] strUrl The HTTP url, i.e. http://localhost:8000/system
     */
    cJSONClientConnector(const std::string& strUrl);
    /// DTOR
    ~cJSONClientConnector();
    /// Disable copy construction
    cJSONClientConnector(const cJSONClientConnector&) = delete;
    /// Disable copy assignment
    cJSONClientConnector& operator=(const cJSONClientConnector&) = delete;
    /**
     * Move constructor
     * @param[in,out] other Move from object *this is created from. Left in empty state.
     */
    cJSONClientConnector(cJSONClientConnector&& other);
    /**
     * Move assignment
     * @param[in,out] other Move from object *this is created from. Left in empty state.
     * @return *this
     */
    cJSONClientConnector& operator=(cJSONClientConnector&& other);

public:
    /**
     * Send an RPC message to the connected url
     * @param[in] message Message to send
     * @param[out] result The result of the response
     * @throws jsonrpc::JsonRpcException in case of a connection error
     */
    void SendRPCMessage(const std::string& message, std::string& result) override;

private:
    class cImplementation;
    cImplementation* m_pImplementation;
};

} // namespace http
} // namespace rpc

#endif // PKG_RPC_JSON_HTTP_H_INCLUDED
