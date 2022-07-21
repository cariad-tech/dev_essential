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

#ifndef PKG_RPC_DETAIL_JSON_RPC_IMPL_H_INCLUDED
#define PKG_RPC_DETAIL_JSON_RPC_IMPL_H_INCLUDED

#include <rpc/json_rpc.h>

namespace rpc {

template <typename Stub, typename Connector, typename ConnectorInitializer>
inline jsonrpc_remote_object<Stub, Connector, ConnectorInitializer>::jsonrpc_remote_object(
    const ConnectorInitializer& oInitializer)
    : Connector(oInitializer), Stub(*static_cast<jsonrpc::IClientConnector*>(this))
{
}

template <typename Stub, typename Connector, typename ConnectorInitializer>
inline Stub& jsonrpc_remote_object<Stub, Connector, ConnectorInitializer>::GetStub() const
{
    return *const_cast<Stub*>(static_cast<const Stub*>(this));
}

template <typename Stub, typename Interface, typename Connector, typename ConnectorInitializer>
inline jsonrpc_remote_interface<Stub, Interface, Connector, ConnectorInitializer>::
    jsonrpc_remote_interface(const ConnectorInitializer& oInitializer)
    : jsonrpc_remote_object<Stub, Connector, ConnectorInitializer>(oInitializer)
{
}

template <typename Stub, typename Interface, typename Connector, typename ConnectorInitializer>
inline Stub& jsonrpc_remote_interface<Stub, Interface, Connector, ConnectorInitializer>::GetStub()
    const
{
    return *const_cast<Stub*>(static_cast<const Stub*>(this));
}

template <typename ServerStub, typename Connector>
inline jsonrpc_object_server<ServerStub, Connector>::jsonrpc_object_server()
    : ServerStub(*static_cast<jsonrpc::AbstractServerConnector*>(this))
{
}

template <typename ServerStub, typename Connector>
inline a_util::result::Result jsonrpc_object_server<ServerStub, Connector>::HandleCall(
    const char* strRequest, size_t nRequestSize, IResponse& oResponse)
{
    try {
        std::string strRequestHelper(strRequest, nRequestSize);
        if (!Connector::OnRequest(strRequestHelper, &oResponse)) {
            return InvalidCall;
        }
    }
    catch (...) {
        return FatalError;
    }

    return {};
}

} // namespace rpc

#endif // PKG_RPC_DETAIL_JSON_RPC_IMPL_H_INCLUDED
