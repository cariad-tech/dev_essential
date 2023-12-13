/**
 * @file
 * RPC Protocol declaration.
 *
 * Copyright @ 2021 VW Group. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla
 * Public License, v. 2.0. If a copy of the MPL was not distributed
 * with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "url.h"

#include <a_util/preprocessor/detail/disable_warnings.h>
#include <rpc/http/json_http_rpc.h>

A_UTIL_DISABLE_COMPILER_WARNINGS
#include <httplib/httplib.h>
A_UTIL_ENABLE_COMPILER_WARNINGS

namespace rpc {
namespace http {
namespace detail {

// this is not the same as httplib::detail::encode_url as it treats spaces differently
std::string url_encode(const std::string& s)
{
    std::string result;

    for (int i = 0; s[i]; i++) {
        switch (s[i]) {
        case ' ':
            result += "%20";
            break;
        case '\'':
            result += "%27";
            break;
        case ',':
            result += "%2C";
            break;
        case ':':
            result += "%3A";
            break;
        case ';':
            result += "%3B";
            break;
#if !(defined(__GNUC__) && __aarch64__)
        default:
            if (s[i] < 0) {
                result += '%';
                char hex[4] = {};
#ifdef _WIN32
                const size_t len = _snprintf_s(hex, _TRUNCATE, "%02X", (unsigned char)s[i]);
#else
                const size_t len = snprintf(hex, sizeof(hex), "%02X", (unsigned char)s[i]);
#endif // _WIN32
                assert(len == 2);
                result.append(hex, len);
            }
            else {
                result += s[i];
            }
            break;
#else  // (defined(__GNUC__) && __aarch64__)
        default:
            result += s[i];
            break;
#endif // !(defined(__GNUC__) && __aarch64__)
        }
    }

    return result;
}

std::string encode_url_path(const std::string& strUrl)
{
    size_t nSlashPosition = strUrl.find('/');
    if (nSlashPosition == std::string::npos) {
        return strUrl;
    }
    nSlashPosition = strUrl.find('/', nSlashPosition + 1);
    if (nSlashPosition == std::string::npos) {
        return strUrl;
    }
    nSlashPosition = strUrl.find('/', nSlashPosition + 1);
    if (nSlashPosition == std::string::npos) {
        return strUrl;
    }
    return strUrl.substr(0, nSlashPosition) + url_encode(strUrl.substr(nSlashPosition));
}

} // namespace detail

cJSONRPCServer::cJSONRPCServer() : cRPCServer("application/json")
{
}

class cJSONClientConnector::cImplementation {
public:
    rpc::cUrl m_oUrl;
    httplib::Client m_oHttpClient;

public:
    cImplementation(const std::string& strUrl)
        : m_oUrl(detail::encode_url_path(strUrl).c_str()),
          m_oHttpClient(m_oUrl.GetAuthority().GetHost().c_str(), m_oUrl.GetAuthority().GetPort())
    {
    }
};

cJSONClientConnector::cJSONClientConnector(const std::string& strUrl)
    : m_pImplementation(new cImplementation(strUrl))
{
}

cJSONClientConnector::~cJSONClientConnector()
{
    delete m_pImplementation;
}

cJSONClientConnector::cJSONClientConnector(cJSONClientConnector&& other)
    : m_pImplementation(other.m_pImplementation)
{
    other.m_pImplementation = nullptr;
}

cJSONClientConnector& cJSONClientConnector::operator=(cJSONClientConnector&& other)
{
    if (this != &other) {
        delete m_pImplementation;
        m_pImplementation = other.m_pImplementation;
        other.m_pImplementation = nullptr;
    }
    return *this;
}

void cJSONClientConnector::SendRPCMessage(const std::string& message, std::string& result)
{
    if (!m_pImplementation) {
        return;
    }

    const std::string url = m_pImplementation->m_oUrl.GetPath().insert(0, 1, '/');
    const char* const content_type = "application/json";

    httplib::Client& http_client = m_pImplementation->m_oHttpClient;
    typedef std::unique_ptr<httplib::Response> Response;
    Response response(http_client.post(url.c_str(), message, content_type));

    if (!response.get()) {
        throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_CONNECTOR,
                                        "error while performing call, invalid response received");
    }

    if (response->status != 200) {
        using a_util::strings::format;
        throw jsonrpc::JsonRpcException(
            jsonrpc::Errors::ERROR_CLIENT_CONNECTOR,
            format("http error while performing call: %d", response->status));
    }

    result = std::move(response->body);
}

} // namespace http
} // namespace rpc
