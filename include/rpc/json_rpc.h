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

#ifndef PKG_RPC_JSON_RPC_H_INCLUDED
#define PKG_RPC_JSON_RPC_H_INCLUDED

#include "rpc/rpc_server.h"

#include <jsonrpccpp/client/iclientconnector.h>
#include <jsonrpccpp/server/abstractserverconnector.h>

namespace rpc {

/**
 * Utility base class to facilitate JSON conversions
 */
class cJSONConversions {
public:
    /**
     * Get the @c Json::Value representation of an object of type @ref a_util::result::Result
     * @param[in] nResult The result object to convert
     * @return Json::Value representation
     */
    static Json::Value result_to_json(a_util::result::Result nResult);
#ifndef PKGRPC_NO_TOSTRING
    /**
     * Convert an unsigned integer to a string
     * @param[in] nValue The value to convert
     * @return String representation of @c nValue
     */
    static std::string to_string(uint64_t nValue);
    /**
     * Convert a signed integer to a string
     * @param[in] nValue The value to convert
     * @return String representation of @c nValue
     */
    static std::string to_string(int64_t nValue);
#endif // PKGRPC_NO_TOSTRING
    /**
     * Get the @ref a_util::result::Result representation of an object of type @c Json::Value
     * @param[in] oValue The json value object to convert
     * @return a_util::result::Result representation
     */
    static a_util::result::Result json_to_result(const Json::Value& oValue);
    /**
     * Convert a string to an unsigned integer
     * @param[in] strValue The value to convert
     * @return Unsigned integer representation of @c strValue
     */
    static uint64_t stoull(const std::string& strValue);
    /**
     * Convert a string to a signed integer
     * @param[in] strValue The value to convert
     * @return Signed integer representation of @c strValue
     */
    static int64_t stoll(const std::string& strValue);
};

/**
 * Template for direct usage of the stub methods
 * @tparam Stub The RPC stub to inherit from
 * @tparam Connector The connector to inherit from
 * @tparam ConnectorInitializer The connector initializer
 */
template <typename Stub, typename Connector, typename ConnectorInitializer>
class jsonrpc_remote_object : private Connector, public Stub {
public:
    /**
     * Constructor
     * @param[in] oInitializer An instance of the @c ConnectorInitializer. If using http then this
     *                         is the HTTP url, e.g. http://localhost:8000/system
     */
    jsonrpc_remote_object(const ConnectorInitializer& oInitializer);

protected:
    /**
     * Access the rpc stub.
     * @return Reference to the stub.
     */
    Stub& GetStub() const;
};

/**
 * Template for mapping interfaces to a client stub.
 * @tparam Stub The RPC stub to inherit from
 * @tparam Interface The interface to inherit from
 * @tparam Connector The connector to inherit from
 * @tparam ConnectorInitializer The connector initializer
 */
template <typename Stub, typename Interface, typename Connector, typename ConnectorInitializer>
class jsonrpc_remote_interface
    : private jsonrpc_remote_object<Stub, Connector, ConnectorInitializer>,
      protected cJSONConversions,
      public Interface {
public:
    /**
     * Constructor
     * @param[in] oInitializer An instance of the @c ConnectorInitializer. If using http then this
     *                         is the HTTP url, e.g. http://localhost:8000/system
     */
    jsonrpc_remote_interface(const ConnectorInitializer& oInitializer);

protected:
    /**
     * Access the rpc stub.
     * @return Reference to the stub.
     */
    Stub& GetStub() const;
};

/**
 * Connector that sends responses via @ref IResponse
 */
class cServerConnector : public jsonrpc::AbstractServerConnector {
public:
    /**
     * Currently not implemented, returns always @c true
     * @retval true
     */
    bool StartListening();
    /**
     * Currently not implemented, returns always @c true
     * @retval true
     */
    bool StopListening();
    /**
     * Sets the @c response to @c addInfo which must be of type @ref IResponse
     * @param[in] response The response message
     * @param[out] addInfo Pointer to object of type @ref IResponse the @c response gets set to
     * @retval true
     */
    bool SendResponse(const std::string& response, void* addInfo);
    /**
     * Called on request
     * @param[in] request The request string
     * @param[out] response The response which gets set
     * @retval true
     */
    bool OnRequest(const std::string& request, IResponse* response);
};

/**
 * Template to implement an RPC object
 * @tparam ServerStub The server stub to inherit from
 * @tparam Connector The connector to inherit from
 */
template <typename ServerStub, typename Connector = cServerConnector>
class jsonrpc_object_server : protected Connector,
                              protected ServerStub,
                              public IRPCObject,
                              protected cJSONConversions {
public:
    /// CTOR
    jsonrpc_object_server();

public:
    /**
     * Handles an RPC call
     * @param[in] strRequest The request message
     * @param[in] nRequestSize Size of the request message
     * @param[out] oResponse Gets the response message set
     */
    virtual a_util::result::Result HandleCall(const char* strRequest,
                                              size_t nRequestSize,
                                              IResponse& oResponse);
};

} // namespace rpc

#endif // PKG_RPC_HTTP_RPC_PROTOCOL_H_INCLUDED

#include "rpc/detail/json_rpc_impl.h"
