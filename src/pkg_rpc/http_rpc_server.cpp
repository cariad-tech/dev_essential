/**
 * @file
 * RPC Protocol implementation.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <rpc/http/http_rpc_server.h>

namespace rpc {
namespace http {
namespace detail {

cRPCServer::cRPCServer(const char* strContentType)
    : cRPCObjectsRegistry(), cThreadedHttpServer(), m_strContentType(strContentType)
{
}

a_util::result::Result cRPCServer::RegisterRPCObject(const char* strName, IRPCObject* pObject)
{
    std::string strURL = std::string("/") + strName;
    return cRPCObjectsRegistry::RegisterRPCObject(strURL.c_str(), pObject);
}

a_util::result::Result cRPCServer::UnregisterRPCObject(const char* strName)
{
    std::string strURL = std::string("/") + strName;
    return cRPCObjectsRegistry::UnregisterRPCObject(strURL.c_str());
}

class cResponse : public IResponse {
private:
    std::string& m_strResponse;

public:
    cResponse(std::string& oResponse) : m_strResponse(oResponse)
    {
    }

    virtual void Set(const char* strResponse, size_t nResponseSize)
    {
        m_strResponse.assign(strResponse, nResponseSize);
    }
};

bool cRPCServer::HandleRequest(const std::string& strName,
                               const std::string& strRequest,
                               std::string& strResponse,
                               std::string& strContentType)
{
    cRPCObjectsRegistry::cLockedRPCObject m_oLockedObject =
        cRPCObjectsRegistry::GetRPCObject(strName.c_str());
    if (m_oLockedObject) {
        strContentType = m_strContentType;
        cResponse oResponse(strResponse);
        a_util::result::Result oRes =
            m_oLockedObject->HandleCall(strRequest.c_str(), strRequest.length(), oResponse);
        if (oRes) {
            return true;
        }
    }
    return false;
}

} // namespace detail
} // namespace http
} // namespace rpc
