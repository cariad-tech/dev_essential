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

#ifndef PKG_RPC_HTTP_RPC_PROTOCOL_H_INCLUDED
#define PKG_RPC_HTTP_RPC_PROTOCOL_H_INCLUDED

#include <rpc/http/threaded_http_server.h>
#include <rpc/rpc_object_registry.h>

namespace rpc {

namespace http {

/** @cond INTERNAL_DOCUMENTATION */
namespace detail {

/**
 * An RPC Server that receives calls via HTTP.
 */
class cRPCServer : private cRPCObjectsRegistry, public cThreadedHttpServer {
public:
    /**
     * Constructor.
     * @param[in] strContentType The content type that should be set for the responses.
     */
    cRPCServer(const char* strContentType);

    /**
     * @copydoc IRPCObjectsRegistry::RegisterRPCObject
     */
    virtual a_util::result::Result RegisterRPCObject(const char* strName, IRPCObject* pObject);

    /**
     * @copydoc IRPCObjectsRegistry::UnregisterRPCObject
     */
    virtual a_util::result::Result UnregisterRPCObject(const char* strName);

protected:
    bool HandleRequest(const std::string& strName,
                       const std::string& strRequest,
                       std::string& strResponse,
                       std::string& strContentType);

private:
    std::string m_strContentType;
};

} // namespace detail
/** @endcond */

} // namespace http
} // namespace rpc

#endif // PKG_RPC_HTTP_RPC_PROTOCOL_H_INCLUDED
