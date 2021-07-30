/**
 * @file
 * RPC launcher
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 *     This Source Code Form is subject to the terms of the Mozilla
 *     Public License, v. 2.0. If a copy of the MPL was not distributed
 *     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * If it is not possible or desirable to put the notice in a particular file, then
 * You may include the notice in a location (such as a LICENSE file in a
 * relevant directory) where a recipient would be likely to look for such a notice.
 *
 * You may add additional accurate notices of copyright ownership.
 */

#include "rpc/http/threaded_http_server.h"

#include "a_util/preprocessor/detail/disable_warnings.h"
#include "rpc/rpc_server.h"
#include "url.h"

#include <deque>
A_UTIL_DISABLE_COMPILER_WARNINGS
#include <httplib/httplib.h>
A_UTIL_ENABLE_COMPILER_WARNINGS
#include <mutex>
#include <thread>

namespace rpc {
namespace http {
namespace detail {

struct AcceptFunc {
    std::deque<std::pair<httplib::Server*, socket_t>> m_queueRequest;
    std::mutex m_csQueue;

    void Push(httplib::Server& oServer, socket_t nSocket)
    {
        m_csQueue.lock();
        m_queueRequest.push_back(std::make_pair(&oServer, nSocket));
        m_csQueue.unlock();
    }
    std::pair<httplib::Server*, socket_t> Pop()
    {
        m_csQueue.lock();
        std::pair<httplib::Server*, socket_t> item = m_queueRequest.front();
        m_queueRequest.pop_front();
        m_csQueue.unlock();
        return item;
    }
    void operator()(httplib::Server& oServer, socket_t nSocket)
    {
        Push(oServer, nSocket);
        std::thread oRequestThread(&AcceptFunc::ProcessRequest, this);
        oRequestThread.detach();
    }

    void ProcessRequest()
    {
        std::pair<httplib::Server*, socket_t> item = Pop();
        item.first->process_request(item.second);
    }
};

class cThreadedHttpServer::cImplementation : private httplib::Server {
public:
    cImplementation(cThreadedHttpServer& oServer) : m_oServer(oServer)
    {
    }

    ~cImplementation()
    {
        StopListening();
    }

    void AcceptServerRequest()
    {
        accept(m_oAcceptFunc);
    }

    a_util::result::Result StartListening(const char* strURL, int reuse)
    {
        cUrl oURL(strURL);
        if (!oURL.IsValid()) {
            RETURN_ERROR_DESCRIPTION(InvalidURL, "The URL %sis not valid", oURL.AsString().c_str());
        }

        if (!listen(oURL.GetAuthority().GetHost().c_str(), oURL.GetAuthority().GetPort(), reuse)) {
            RETURN_ERROR_DESCRIPTION(StartupFailed, "Unable to start http server on %s", strURL);
        }

        m_pAcceptThread.reset(
            new std::thread(&cThreadedHttpServer::cImplementation::AcceptServerRequest, this));

        return {};
    }

    a_util::result::Result StopListening()
    {
        if (m_pAcceptThread && m_pAcceptThread->joinable()) {
            stop();
            m_pAcceptThread->join();
            m_pAcceptThread.reset();
        }

        return {};
    }

protected:
    bool handle_request(const httplib::Request& oRequest, httplib::Response& oResponse) override
    {
        std::string strContentType;
        bool bResult =
            m_oServer.HandleRequest(oRequest.url, oRequest.body, oResponse.body, strContentType);
        oResponse.set_header("Content-Type", strContentType.c_str());
        return bResult;
    }

protected:
    // std::thread m_oAcceptThread;
    std::unique_ptr<std::thread> m_pAcceptThread;
    AcceptFunc m_oAcceptFunc;
    cThreadedHttpServer& m_oServer;
};

cThreadedHttpServer::cThreadedHttpServer() : m_pImplementation(new cImplementation(*this))
{
}

cThreadedHttpServer::~cThreadedHttpServer() = default;

a_util::result::Result cThreadedHttpServer::StartListening(const char* strURL, int reuse)
{
    return m_pImplementation->StartListening(strURL, reuse);
}

a_util::result::Result cThreadedHttpServer::StopListening()
{
    return m_pImplementation->StopListening();
}

} // namespace detail
} // namespace http
} // namespace rpc
