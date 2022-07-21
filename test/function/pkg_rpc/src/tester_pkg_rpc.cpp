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

#include <a_util/strings/strings_functions.h>
#include <rpc/rpc.h>

#include <testclientstub.h>
#include <testserverstub.h>

#ifdef _WIN32
#ifndef __MINGW32__
#define NOMINMAX
#endif // __MINGW32__
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif // _WIN32

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <future>
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
#ifdef _WIN32
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

#ifdef _WIN32
    shutdown(sock_fd, SD_BOTH);
#else
    shutdown(sock_fd, SHUT_RDWR);
#endif

    rpc::http::cJSONRPCServer rpc_server;
    ASSERT_TRUE(isOk(rpc_server.StartListening("http://0.0.0.0:9090")));
    ASSERT_TRUE(isOk(rpc_server.StopListening()));

#ifdef _WIN32
    closesocket(sock_fd);
#else
    close(sock_fd);
#endif
}

class cTestServerWithDelay : public cTestServer {
public:
    cTestServerWithDelay(rpc::http::cJSONRPCServer& oServer, bool& function_returned)
        : cTestServer(oServer), _function_returned(function_returned)
    {
    }

    int GetInteger(int nValue) override
    {
        // inform that rpc callback is executed
        _rpc_called.set_value();

        // wait until the other thread will call the destructor
        _destructor_called.wait();

        _function_returned = true;
        return nValue;
    }
    std::promise<void> _rpc_called;
    std::shared_future<void> _destructor_called;
    bool& _function_returned;
    const size_t server_size = 10;
};

/*
 * Constructs 10 RPC servers, each one having service id from test0 to test9
 * StopListening of threaded_rpc_server should first wait all detached threads to finish
 */
TEST(HttpServer, WaitDetachedThreads)
{
    std::unique_ptr<rpc::http::cJSONRPCServer> rpc_server =
        std::make_unique<rpc::http::cJSONRPCServer>();
    constexpr size_t server_size = 10;

    std::array<bool, server_size> _function_returned{false};
    std::array<std::unique_ptr<cTestServerWithDelay>, server_size> _server_array;
    std::array<std::unique_ptr<cTestClient>, server_size> _client_array;
    std::promise<void> _destructor_called;
    std::shared_future<void> _destructor_called_future = _destructor_called.get_future();
    std::array<std::future<void>, server_size> _rpc_called;

    for (size_t i = 0; i < server_size; ++i) {
        _server_array[i] =
            std::make_unique<cTestServerWithDelay>(*rpc_server, _function_returned[i]);
        const std::string service_name = "test" + a_util::strings::toString(i);

        _server_array[i]->_destructor_called = _destructor_called_future;
        _rpc_called[i] = _server_array[i]->_rpc_called.get_future();

        ASSERT_TRUE(a_util::result::isOk(
            rpc_server->RegisterRPCObject(service_name.c_str(), _server_array[i].get())));

        const std::string client_url = "http://127.0.0.1:1234/" + service_name;
        _client_array[i] = std::make_unique<cTestClient>(client_url.c_str());
    }

    ASSERT_TRUE(a_util::result::isOk(rpc_server->StartListening("http://127.0.0.1:1234")));

    std::array<std::thread, server_size> _thread_array;

    // launch threads each calling a separate client
    for (size_t i = 0; i < server_size; ++i) {
        _thread_array[i] = std::thread([&, i]() { _client_array[i]->GetInteger(1234); });
    }

    // wait that all clients are inside the callback function
    for (size_t i = 0; i < server_size; ++i) {
        _rpc_called[i].wait();
    }
    // inform clients to unblock the callback
    _destructor_called.set_value();

    for (size_t i = 0; i < server_size; ++i) {
        const std::string service_name = "test" + a_util::strings::toString(i);
        rpc_server->UnregisterRPCObject(service_name.c_str());
    }

    rpc_server.reset();
    // all client callback funtions should return (not really return but continue)
    for (size_t i = 0; i < server_size; ++i) {
        ASSERT_TRUE(_function_returned[i]);
    }

    for (size_t i = 0; i < server_size; ++i) {
        _thread_array[i].join();
    }
}
