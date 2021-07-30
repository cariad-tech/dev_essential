/**
 * @file
 * RPC Package functional tester implementation file.
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

#include "a_util/strings/strings_functions.h"
#include "rpc/rpc.h"

#include <testclientstub.h>
#include <testserverstub.h>

#ifdef WIN32
#define NOMINMAX
#include "winsock2.h"
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <cstdint>
#include <gtest/gtest.h>
#include <limits>

typedef rpc::
    jsonrpc_remote_object<rpc_stubs::cTestClientStub, rpc::http::cJSONClientConnector, std::string>
        cTestClient;

class cTestServer : public rpc::jsonrpc_object_server<rpc_stubs::cTestServerStub> {
public:
    cTestServer(rpc::http::cJSONRPCServer& oServer) : m_oServer(oServer)
    {
    }

    virtual int GetInteger(int nValue)
    {
        return nValue;
    }

    virtual std::string Concat(const std::string& strString1, const std::string& strString2)
    {
        return strString1 + strString2;
    }

    virtual std::string GetIntegerAsString(const std::string& nValue)
    {
        return to_string(stoll(nValue));
    }

    a_util::result::Result GenerateError()
    {
        RETURN_ERROR_DESCRIPTION(-2, "test error");
    }

    virtual Json::Value GetResult()
    {
        return result_to_json(GenerateError());
    }

    virtual Json::Value RegisterObject()
    {
        m_pChildServer = std::make_unique<cTestServer>(m_oServer);
        return result_to_json(m_oServer.RegisterRPCObject("test_register", m_pChildServer.get()));
    }

    virtual Json::Value UnregisterObject()
    {
        return result_to_json(m_oServer.UnregisterRPCObject("test_register"));
    }

    virtual Json::Value UnregisterSelf()
    {
        // this deadlocks
        return result_to_json(m_oServer.UnregisterRPCObject("test"));
    }

private:
    rpc::http::cJSONRPCServer& m_oServer;
    std::unique_ptr<cTestServer> m_pChildServer;
};

/**
 * @req_id #34310
 */
TEST(cTesterPkgRpc, TestCalls)
{
    rpc::http::cJSONRPCServer rpc_server;
    cTestServer oTestServer(rpc_server);
    ASSERT_TRUE(a_util::result::isOk(rpc_server.RegisterRPCObject("test", &oTestServer)));
    ASSERT_TRUE(a_util::result::isOk(rpc_server.StartListening("http://127.0.0.1:1234")));

    cTestClient oClient("http://127.0.0.1:1234/test");

    ASSERT_EQ(oClient.GetInteger(1234), 1234);
    ASSERT_EQ(oClient.Concat("foo", "bar"), "foobar");

#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
    const std::string strValue =
        rpc::cJSONConversions::to_string(std::numeric_limits<std::int64_t>::max());
#if defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))
#pragma GCC diagnostic pop
#endif // defined(__GNUC__) && ((__GNUC__ == 5) && (__GNUC_MINOR__ == 2))

    ASSERT_EQ(oClient.GetIntegerAsString(strValue), strValue);

    a_util::result::Result nResult = rpc::cJSONConversions::json_to_result(oClient.GetResult());

    // LOG_RESULT(nResult);
    ASSERT_TRUE(nResult.getErrorCode() == a_util::result::Result(-2).getErrorCode());
    ASSERT_TRUE(a_util::strings::isEqual(nResult.getDescription(), "test error"));

    ASSERT_TRUE(std::string(nResult.getFunction()).find("GenerateError") != std::string::npos);
    ASSERT_TRUE(a_util::strings::isEqual(nResult.getFile(), __FILE__));
    ASSERT_TRUE(nResult.getLine() > 0 && nResult.getLine() < __LINE__);

    ASSERT_TRUE(isOk(rpc::cJSONConversions::json_to_result(oClient.RegisterObject())));
    {
        cTestClient oRegisteredObjectClient("http://127.0.0.1:1234/test_register");
        ASSERT_TRUE(oRegisteredObjectClient.GetInteger(1234) == 1234);
    }
    ASSERT_TRUE(isOk(rpc::cJSONConversions::json_to_result(oClient.UnregisterObject())));

    ASSERT_TRUE(isOk(rpc_server.UnregisterRPCObject("test")));
}

/**
 * @req_id"#34310
 */
TEST(cTesterPkgRpc, TestObjectNameWithSpaces)
{
#define TEST_OBJ_STRING "test with spaces/and even more spaces"
    rpc::http::cJSONRPCServer rpc_server;
    cTestServer oTestServer(rpc_server);
    ASSERT_TRUE(isOk(rpc_server.RegisterRPCObject(TEST_OBJ_STRING, &oTestServer)));
    ASSERT_TRUE(isOk(rpc_server.StartListening("http://127.0.0.1:1234")));

    cTestClient oClient("http://127.0.0.1:1234/" TEST_OBJ_STRING);
    ASSERT_TRUE(oClient.GetInteger(1234) == 1234);
}

/**
 * @brief Create two http servers to the same port and check if the second one fails; and a third
 * one to another port.
 */
TEST(HttpServer, TestPortBindingNoReuse)
{
    rpc::http::cJSONRPCServer rpc_server_1, rpc_server_2, rpc_server_3;

    ASSERT_TRUE(isOk(rpc_server_1.StartListening("http://0.0.0.0:9090", 0)));
    ASSERT_FALSE(isOk(rpc_server_2.StartListening("http://0.0.0.0:9090", 0)));
    ASSERT_TRUE(isOk(rpc_server_3.StartListening("http://0.0.0.0:9091", 0)));

    ASSERT_TRUE(isOk(rpc_server_1.StopListening()));
    ASSERT_TRUE(isOk(rpc_server_3.StopListening()));
}

/**
 * @brief Create "manually" a server socket and keep it in a shutdowned state and an http server to
 * the same port with reuse option.
 */
TEST(HttpServer, TestPortBindingReuse)
{
    auto sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(sock_fd, -1);

    // Make 'reuse address' option available
    int reuse = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
#ifdef WIN32
    reuse ^= 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&reuse, sizeof(reuse));
#endif
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    u_short port = 9090;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    ASSERT_EQ(bind(sock_fd, (struct sockaddr*)&address, sizeof(address)), 0);
    ASSERT_EQ(listen(sock_fd, 1), 0);

#ifdef _MSC_VER
    shutdown(sock_fd, SD_BOTH);
#else
    shutdown(sock_fd, SHUT_RDWR);
#endif

    rpc::http::cJSONRPCServer rpc_server;
    ASSERT_TRUE(isOk(rpc_server.StartListening("http://0.0.0.0:9090")));
    ASSERT_TRUE(isOk(rpc_server.StopListening()));

#ifdef _MSC_VER
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif
}
