/**
 * @file
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

#include <rpc/json_rpc.h>

#if defined(__QNX__) && defined(__GNUC__) && (__GNUC__ == 5)
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#else
#define HAVE_STD_TO_STRING
#define HAVE_STD_STOLL
#endif

namespace rpc {

Json::Value cJSONConversions::result_to_json(a_util::result::Result nResult)
{
    Json::Value oResult;
    oResult["ErrorCode"] = nResult.getErrorCode();
    oResult["Description"] = nResult.getDescription();
    oResult["Line"] = nResult.getLine();
    oResult["File"] = nResult.getFile();
    oResult["Function"] = nResult.getFunction();
    return oResult;
}

std::string cJSONConversions::to_string(uint64_t nValue)
{
#ifdef HAVE_STD_TO_STRING
    return std::to_string(nValue);
#else
    // Bug in g++ 5.x.x header file, not having std::to_string for C++11
    std::ostringstream stream;
    stream << nValue;
    return stream.str();
#endif
}

std::string cJSONConversions::to_string(int64_t nValue)
{
#ifdef HAVE_STD_TO_STRING
    return std::to_string(nValue);
#else
    // Bug in g++ 5.x.x header file, not having std::to_string for C++11
    std::ostringstream stream;
    stream << nValue;
    return stream.str();
#endif
}

a_util::result::Result cJSONConversions::json_to_result(const Json::Value& oValue)
{
    return a_util::result::Result(oValue["ErrorCode"].asInt(),
                                  oValue["Description"].asString().c_str(),
                                  oValue["Line"].asInt(),
                                  oValue["File"].asString().c_str(),
                                  oValue["Function"].asString().c_str());
}

int64_t cJSONConversions::stoll(const std::string& strValue)
{
#ifdef HAVE_STD_STOLL
    return std::stoll(strValue);
#else
    // Bug in g++ 5.x.x header file, not having std::sto(u)ll for C++11
    return strtoll(strValue.c_str(), 0, 10);
#endif
}

uint64_t cJSONConversions::stoull(const std::string& strValue)
{
#ifdef HAVE_STD_STOLL
    return std::stoull(strValue);
#else
    // Bug in g++ 5.x.x header file, not having std::sto(u)ll for C++11
    return strtoull(strValue.c_str(), 0, 10);
#endif
}

bool cServerConnector::StartListening()
{
    return true;
}

bool cServerConnector::StopListening()
{
    return true;
}

bool cServerConnector::SendResponse(const std::string& response, void* addInfo)
{
    IResponse* pResponse = static_cast<IResponse*>(addInfo);
    pResponse->Set(response.data(), response.size());
    return true;
}

bool cServerConnector::OnRequest(const std::string& request, IResponse* response)
{
    std::string response_value;
    ProcessRequest(request, response_value);
    response->Set(response_value.c_str(), response_value.size());
    return true;
}

} // namespace rpc
