/**
 * @file
 * RPC Launcher.
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

#ifndef PKG_RPC_RPC_DETAIL_THREAD_HTTP_SERVER_H_
#define PKG_RPC_RPC_DETAIL_THREAD_HTTP_SERVER_H_

#include "a_util/result/result_type.h"

#include <memory>
#include <string>

namespace rpc {
namespace http {

/** @cond INTERNAL_DOCUMENTATION */
namespace detail {

class cThreadedHttpServer {
public:
    cThreadedHttpServer();
    ~cThreadedHttpServer();
    cThreadedHttpServer(const cThreadedHttpServer&) = delete;
    cThreadedHttpServer& operator=(const cThreadedHttpServer&) = delete;
    cThreadedHttpServer(cThreadedHttpServer&&) = default;
    cThreadedHttpServer& operator=(cThreadedHttpServer&&) = default;

    /**
     * Starts listening and processing of requests.
     * @param[in] strURL The URL, i.e. http://0.0.0.0:8000
     * @return Standard result
     */
    a_util::result::Result StartListening(const char* strURL, int reuse = 1);

    /**
     * Stop processing of requests.
     * @return Standard result
     */
    a_util::result::Result StopListening();

protected:
    virtual bool HandleRequest(const std::string& strUrl,
                               const std::string& strRequest,
                               std::string& strResponse,
                               std::string& strContentType) = 0;

private:
    class cImplementation;
    std::unique_ptr<cImplementation> m_pImplementation;
};

} // namespace detail
/** @endcond */
} // namespace http
} // namespace rpc

#endif
