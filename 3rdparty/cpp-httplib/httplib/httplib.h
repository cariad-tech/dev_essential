//
//  httplib.h
//
//  Copyright (c) 2012 Yuji Hirose. All rights reserved.
//  The Boost Software License 1.0

#ifndef _CPPHTTPLIB_HTTPSLIB_H_
#define _CPPHTTPLIB_HTTPSLIB_H_

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#ifndef SO_SYNCHRONOUS_NONALERT
#define SO_SYNCHRONOUS_NONALERT 0x20
#endif
#ifndef SO_OPENTYPE
#define SO_OPENTYPE 0x7008
#endif
#if (_MSC_VER < 1900)
#define snprintf _snprintf_s
#endif

#define S_ISREG(m)  (((m)&S_IFREG)==S_IFREG)
#define S_ISDIR(m)  (((m)&S_IFDIR)==S_IFDIR)

#endif // _MSC_VER

#ifdef _WIN32

#include <fcntl.h>
#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#undef min
#undef max

typedef SOCKET socket_t;

#else

#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h> // fd_set

typedef int socket_t;

#endif // _WIN32

#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

namespace httplib
{

template <typename T>
std::string to_string(const T& item)
{
    std::ostringstream helper;
    helper << item;
    return helper.str();
}

typedef std::map<std::string, std::string>      Map;
typedef std::multimap<std::string, std::string> MultiMap;

struct Request {
    std::string method;
    std::string url;
    MultiMap    headers;
    std::string body;
    Map         params;

    bool has_header(const char* key) const;
    std::string get_header_value(const char* key) const;
    void set_header(const char* key, const char* val);

    bool has_param(const char* key) const;
};

struct Response {
    int         status;
    MultiMap    headers;
    std::string body;

    bool has_header(const char* key) const;
    std::string get_header_value(const char* key) const;
    void set_header(const char* key, const char* val);

    void set_redirect(const char* url);
    void set_content(const char* s, size_t n, const char* content_type);
    void set_content(const std::string& s, const char* content_type);

    Response() : status(-1) {}
};

class Server {
public:
    Server();

    bool listen(const char* host, int port, int reuse);
    template <typename ProcessFunctor>
    void accept(ProcessFunctor& processor);
    void stop();
    void process_request(socket_t sock);

protected:
    virtual bool handle_request(const Request&, Response&) = 0;

private:
    socket_t    svr_sock_;
    volatile bool keep_accepting;
};

class Client {
public:
    Client(const char* host, int port);

    Response* get(const char* url);
    Response* head(const char* url);
    Response* post(const char* url, const std::string& body, const char* content_type);
    Response* post(const char* url, const void* body, size_t body_size, const char* content_type);
    Response* post(const char* url, const Map& params);

    bool send(const Request& req, Response& res);

private:
    const std::string host_;
    const int         port_;
};

// Implementation
namespace detail {

inline bool socket_read(socket_t sock, char* ptr, size_t size)
{
    while (size)
    {
        int bytes_read = recv(sock, ptr, size, 0);
        if (bytes_read <= 0)
        {
            return false;
        }
        ptr += bytes_read;
        size -= bytes_read;
    }

    return true;
}

inline void socket_write(socket_t sock, const char* ptr, size_t size = static_cast<size_t>(-1))
{
    if (size == static_cast<size_t>(-1)) {
        size = strlen(ptr);
    }

    while (size)
    {
        int bytes_written = send(sock, ptr, size, 0);
        if (bytes_written <= 0)
        {
            return;
        }

        ptr += bytes_written;
        size -= bytes_written;
    }
}

inline bool socket_gets(socket_t sock, char* buf, int bufsiz)
{
    // TODO: buffering for better performance
    int i = 0;

    for (; i < bufsiz - 1;) {
        char byte;
        int n = recv(sock, &byte, 1, 0);

        if (n < 1) {
            if (i == 0) {
                return false;
            } else {
                break;
            }
        }

        buf[i++] = byte;

        if (byte == '\n') {
            break;
        }
    }

    buf[i] = '\0';
    return true;
}

inline void socket_printf(socket_t sock, const char* fmt, ...)
{
    char buf[BUFSIZ];

    va_list args;
    va_start(args, fmt);

    int n = vsnprintf(buf, BUFSIZ, fmt, args);

    va_end(args);

    if (n > 0) {
        if (n >= BUFSIZ) {
            // TODO: buffer size is not large enough...
        } else {
            socket_write(sock, buf, n);
        }
    }
}

inline int close_socket(socket_t sock)
{
#ifdef _MSC_VER
    return closesocket(sock);
#else
    return close(sock);
#endif
}

template <typename T>
inline bool read_and_close_socket(socket_t sock, T& callback, bool reset_connection = true)
{
    bool ret = callback(sock);

    if (reset_connection)
    {
        struct linger so_linger;
        so_linger.l_onoff = 1;
        so_linger.l_linger = 0;
        setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&so_linger, sizeof (so_linger));
    }

    close_socket(sock);
    return ret;
}

inline bool wait_for_socket_readable(socket_t sock, size_t timeout_us)
{
    fd_set file_descriptors;
    FD_ZERO(&file_descriptors);
    FD_SET(sock, &file_descriptors);
#ifdef _MSC_VER
    int max_descriptor = 0;
#else
    int max_descriptor = (int) sock + 1;
#endif
    struct timeval timeout;
    timeout.tv_sec = timeout_us / 1000000;
    timeout.tv_usec = timeout_us % 1000000;

    if (select(max_descriptor, &file_descriptors, NULL, NULL, &timeout) < 0)
    {
        return false;
    }

    return FD_ISSET(sock, &file_descriptors);
}

inline int shutdown_socket(socket_t sock)
{
#ifdef _WIN32
    return shutdown(sock, SD_BOTH);
#else
    return shutdown(sock, SHUT_RDWR);
#endif // _WIN32
}

inline void enable_tcp_no_delay(socket_t sock)
{
    int tcp_no_delay = 1;
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&tcp_no_delay, sizeof(tcp_no_delay));
}

inline socket_t create_socket(const char* host, int port, bool server, int reuse)
{
    // Get address info
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    std::string service = to_string(port);

    if (getaddrinfo(host, service.c_str(), &hints, &result)) {
        return -1;
    }

    for (addrinfo* rp = result; rp; rp = rp->ai_next) {
       // Create a socket
       int sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
       if (sock == -1) {
          continue;
       }

       // we want faster RTTs
       enable_tcp_no_delay(sock);

       // Make 'reuse address' option available
       setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
#ifdef _WIN32
       reuse ^= 1;
       setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&reuse, sizeof(reuse));
#endif

       // bind or connect
       bool done = false;
       if (server) {
           if (::bind(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
               if (listen(sock, 5) == 0) { // Listen through 5 channels
                   done = true;
               }
           }
       } else {
           if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) { // Listen through 5 channels
               done = true;
           }
       }
       if (done) {
          freeaddrinfo(result);
          return sock;
       }

       close_socket(sock);
    }

    freeaddrinfo(result);
    return -1;
}

inline socket_t create_server_socket(const char* host, int port, int reuse)
{
    return create_socket(host, port, true, reuse);
}

inline socket_t create_client_socket(const char* host, int port)
{
    return create_socket(host, port, false, 1);
}

inline bool is_file(const std::string& s)
{
    struct stat st;
    return stat(s.c_str(), &st) >= 0 && S_ISREG(st.st_mode);
}

inline bool is_dir(const std::string& s)
{
    struct stat st;
    return stat(s.c_str(), &st) >= 0 && S_ISDIR(st.st_mode);
}

inline const char* status_message(int status)
{
    switch (status) {
    case 200: return "OK";
    case 400: return "Bad Request";
    case 404: return "Not Found";
    default:
        case 500: return "Internal Server Error";
    }
}

inline const char* get_header_value(const MultiMap& map, const char* key, const char* def)
{
    MultiMap::const_iterator it = map.find(key);
    if (it != map.end()) {
        return it->second.c_str();
    }
    return def;
}

inline int get_header_value_int(const MultiMap& map, const char* key, int def)
{
    MultiMap::const_iterator it = map.find(key);
    if (it != map.end()) {
        return atoi(it->second.c_str());
    }
    return def;
}

inline bool read_headers(socket_t sock, MultiMap& headers)
{
    const size_t BUFSIZ_HEADER = 2048;
    char buf[BUFSIZ_HEADER];

    for (;;) {
        if (!socket_gets(sock, buf, BUFSIZ_HEADER)) {
            return false;
        }
        if (!strcmp(buf, "\r\n")) {
            break;
        }

        std::string header_line(buf);
        size_t column_position = header_line.find(':');
        if (column_position != std::string::npos)
        {
            std::string key = header_line.substr(0, column_position);
            std::string val = header_line.substr(column_position + 1, header_line.length() - 2 /* \r\n */ - column_position - 1);
            while (!val.empty() && val[0] == ' ')
            {
                val = val.substr(1);
            }
            headers.insert(std::make_pair(key, val));
        }
    }

    return true;
}

template <typename T>
bool read_content(socket_t sock, T& x)
{
    int len = get_header_value_int(x.headers, "Content-Length", 0);
    if (len) {
        x.body.assign(len, 0);
        if (!socket_read(sock, &x.body[0], x.body.size())) {
            return false;
        }
    }
    return true;
}

template <typename T>
inline void write_headers(socket_t sock, const T& res)
{
    socket_write(sock, "Connection: close\r\n");

    for (MultiMap::const_iterator x = res.headers.begin(); x != res.headers.end(); ++x) {
        if (x->first != "Content-Type" && x->first != "Content-Length") {
            socket_printf(sock, "%s: %s\r\n", x->first.c_str(), x->second.c_str());
        }
    }

    const char* t = get_header_value(res.headers, "Content-Type", "text/plain");
    socket_printf(sock, "Content-Type: %s\r\n", t);
    socket_printf(sock, "Content-Length: %ld\r\n", res.body.size());
    socket_write(sock, "\r\n");
}

inline void write_response(socket_t sock, const Request& req, const Response& res)
{
    socket_printf(sock, "HTTP/1.0 %d %s\r\n", res.status, status_message(res.status));

    write_headers(sock, res);

    if (!res.body.empty() && req.method != "HEAD") {
        socket_write(sock, res.body.c_str(), res.body.size());
    }
}

inline std::string encode_url(const std::string& s)
{
    std::string result;

    for (int i = 0; s[i]; i++) {
        switch (s[i]) {
        case ' ':  result += "+"; break;
        case '\'': result += "%27"; break;
        case ',':  result += "%2C"; break;
        case ':':  result += "%3A"; break;
        case ';':  result += "%3B"; break;
        default:
            if (s[i] < 0) {
                result += '%';
                char hex[4];
                size_t len = snprintf(hex, sizeof(hex), "%02X", (unsigned char)s[i]);
                assert(len == 2);
                result.append(hex, len);
            } else {
                result += s[i];
            }
            break;
        }
   }

    return result;
}

inline bool is_hex(char c, int& v)
{
    if (0x20 <= c && isdigit(c)) {
        v = c - '0';
        return true;
    } else if ('A' <= c && c <= 'F') {
        v = c - 'A' + 10;
        return true;
    } else if ('a' <= c && c <= 'f') {
        v = c - 'a' + 10;
        return true;
    }
    return false;
}

inline int from_hex_to_i(const std::string& s, int i, int cnt, int& val)
{
    val = 0;
    for (; s[i] && cnt; i++, cnt--) {
        int v = 0;
        if (is_hex(s[i], v)) {
            val = val * 16 + v;
        } else {
            break;
        }
    }
    return --i;
}

inline size_t to_utf8(int code, char* buff)
{
    if (code < 0x0080) {
        buff[0] = (code & 0x7F);
        return 1;
    } else if (code < 0x0800) {
        buff[0] = (0xC0 | ((code >> 6) & 0x1F));
        buff[1] = (0x80 | (code & 0x3F));
        return 2;
    } else if (code < 0xD800) {
        buff[0] = (0xE0 | ((code >> 12) & 0xF));
        buff[1] = (0x80 | ((code >> 6) & 0x3F));
        buff[2] = (0x80 | (code & 0x3F));
        return 3;
    } else if (code < 0xE000)  { // D800 - DFFF is invalid...
        return 0;
    } else if (code < 0x10000) {
        buff[0] = (0xE0 | ((code >> 12) & 0xF));
        buff[1] = (0x80 | ((code >> 6) & 0x3F));
        buff[2] = (0x80 | (code & 0x3F));
        return 3;
    } else if (code < 0x110000) {
        buff[0] = (0xF0 | ((code >> 18) & 0x7));
        buff[1] = (0x80 | ((code >> 12) & 0x3F));
        buff[2] = (0x80 | ((code >> 6) & 0x3F));
        buff[3] = (0x80 | (code & 0x3F));
        return 4;
    }

    // NOTREACHED
    return 0;
}

inline std::string decode_url(const std::string& s)
{
    std::string result;

    for (int i = 0; s[i]; i++) {
        if (s[i] == '%') {
            i++;
            assert(s[i]);

            if (s[i] == '%') {
                result += s[i];
            } else if (s[i] == 'u') {
                // Unicode
                i++;
                assert(s[i]);

                int val = 0;
                i = from_hex_to_i(s, i, 4, val);

                char buff[4];
                size_t len = to_utf8(val, buff);

                if (len > 0) {
                    result.append(buff, len);
                }
            } else {
                // HEX
                int val = 0;
                i = from_hex_to_i(s, i, 2, val);
                result += val;
            }
        } else if (s[i] == '+') {
            result += ' ';
        } else {
            result += s[i];
        }
    }

    return result;
}

inline void write_request(socket_t sock, const Request& req)
{
    std::string url = encode_url(req.url);
    socket_printf(sock, "%s %s HTTP/1.0\r\n", req.method.c_str(), url.c_str());

    write_headers(sock, req);

    if (!req.body.empty()) {
        if (req.has_header("application/x-www-form-urlencoded")) {
            std::string str = encode_url(req.body);
            socket_write(sock, str.c_str(), str.size());
        } else {
            socket_write(sock, req.body.c_str(), req.body.size());
        }
    }
}

template <class Fn>
void split(const char* b, const char* e, char d, Fn& fn)
{
    int i = 0;
    int beg = 0;

    while (e ? (b + i != e) : (b[i] != '\0')) {
        if (b[i] == d) {
            fn(&b[beg], &b[i]);
            beg = i + 1;
        }
        i++;
    }

    if (i) {
        fn(&b[beg], &b[i]);
    }
}

struct ValueFunctor
{
    std::string& key;
    std::string& val;

    ValueFunctor(std::string& key, std::string& val):
        key(key), val(val) {
    }

    void operator()(const char* b, const char* e) {
        if (key.empty()) {
            key.assign(b, e);
        } else {
            val.assign(b, e);
        }
    }
};

struct ElementFunctor
{
    Map& params;
    ElementFunctor(Map& params):
        params(params) {
    }

    void operator()(const char* b, const char* e) {
        std::string key;
        std::string val;
        ValueFunctor splitter(key, val);
        split(b, e, '=', splitter);
        params[key] = detail::decode_url(val);
    }
};

inline void parse_query_text(const std::string& s, Map& params)
{
    ElementFunctor splitter(params);
    split(&s[0], &s[s.size()], '&', splitter);
}

inline bool read_request_line(socket_t sock, Request& req)
{
    const size_t BUFSIZ_REQUESTLINE = 2048;
    char buf[BUFSIZ_REQUESTLINE];
    if (!detail::socket_gets(sock, buf, BUFSIZ_REQUESTLINE)) {
        return false;
    }

    std::string request_line(buf);

    size_t first_space = request_line.find(' ');
    if (first_space == std::string::npos)
    {
        return false;
    }

    std::string method = request_line.substr(0, first_space);

    size_t url_start = first_space + 1;
    size_t url_end = request_line.find_first_of(" ?", url_start);
    std::string url = decode_url(request_line.substr(url_start, url_end - url_start));

    if (!method.empty() && !url.empty())
    {
        req.method = method;
        req.url = url;
        if (request_line.substr(url_end, 1) == "?")
        {
            size_t second_space = request_line.find(' ', url_end);
            std::string parameters = request_line.substr(url_end + 1, second_space - url_end -1);
            detail::parse_query_text(parameters, req.params);
        }
        return true;
    }

    return false;
}

inline bool read_response_line(socket_t sock, Response& res)
{
    const size_t BUFSIZ_RESPONSELINE = 2048;
    char buf[BUFSIZ_RESPONSELINE];
    if (!detail::socket_gets(sock, buf, BUFSIZ_RESPONSELINE)) {
        return false;
    }

    std::string response_line(buf);

    size_t first_space = response_line.find(' ');
    size_t second_space = response_line.find(' ', first_space + 1);
    std::string status_code = response_line.substr(first_space + 1, second_space - first_space - 1);
    res.status = atoi(status_code.c_str());

    return true;
}

#ifdef _MSC_VER
class WSInit {
public:
    WSInit() {
        WSADATA wsaData;
        WSAStartup(0x0002, &wsaData);
    }

    ~WSInit() {
        WSACleanup();
    }
};

static WSInit wsinit_;
#endif

} // namespace detail

// Request implementation
inline bool Request::has_header(const char* key) const
{
    return headers.find(key) != headers.end();
}

inline std::string Request::get_header_value(const char* key) const
{
    return detail::get_header_value(headers, key, "");
}

inline void Request::set_header(const char* key, const char* val)
{
    headers.insert(std::make_pair(key, val));
}

inline bool Request::has_param(const char* key) const
{
    return params.find(key) != params.end();
}

// Response implementation
inline bool Response::has_header(const char* key) const
{
    return headers.find(key) != headers.end();
}

inline std::string Response::get_header_value(const char* key) const
{
    return detail::get_header_value(headers, key, "");
}

inline void Response::set_header(const char* key, const char* val)
{
    headers.insert(std::make_pair(key, val));
}

inline void Response::set_redirect(const char* url)
{
    set_header("Location", url);
    status = 302;
}

inline void Response::set_content(const char* s, size_t n, const char* content_type)
{
    body.assign(s, n);
    set_header("Content-Type", content_type);
}

inline void Response::set_content(const std::string& s, const char* content_type)
{
    body = s;
    set_header("Content-Type", content_type);
}

// HTTP server implementation
inline Server::Server()
    : svr_sock_(-1), keep_accepting(true)
{
}

struct ProcessFunctor
{
    void operator()(Server& server, socket_t sock) {
        server.process_request(sock);
    }
};

inline bool Server::listen(const char* host, int port, int reuse)
{
    svr_sock_ = detail::create_server_socket(host, port, reuse);
    if (svr_sock_ == static_cast<socket_t>(-1)) {
        return false;
    }
    keep_accepting = true;
    return true;
}

template <typename ProcessFunctor>
inline void Server::accept(ProcessFunctor& processor)
{
    while (keep_accepting) {
        if (detail::wait_for_socket_readable(svr_sock_, 100000)) {
            socket_t sock = ::accept(svr_sock_, NULL, NULL);

            if (sock == static_cast<socket_t>(-1)) {
                if (svr_sock_ != static_cast<socket_t>(-1)) {
                    detail::close_socket(svr_sock_);
                } else {
                    ; // The server socket was closed by user.
                }
                break;
            }

            detail::enable_tcp_no_delay(sock);

            processor(*this, sock);
        }
    }
}

inline void Server::stop()
{
    keep_accepting = false;
    detail::shutdown_socket(svr_sock_);
    detail::close_socket(svr_sock_);
    svr_sock_ = -1;
}

inline void Server::process_request(socket_t sock)
{
    while (detail::wait_for_socket_readable(sock, 10000000))
    {
        Request req;
        Response res;

        if (!detail::read_request_line(sock, req) ||
            !detail::read_headers(sock, req.headers)) {
            break;
        }

        if (req.method == "POST") {
            if (!detail::read_content(sock, req)) {
                break;
            }
            static std::string type = "application/x-www-form-urlencoded";
            if (!req.get_header_value("Content-Type").compare(0, type.size(), type)) {
                detail::parse_query_text(req.body, req.params);
            }
        }

        if (handle_request(req, res)) {
            if (res.status == -1) {
                res.status = 200;
            }
        } else {
            res.status = 404;
        }

        assert(res.status != -1);

        detail::write_response(sock, req, res);
    }

    detail::close_socket(sock);
}

// HTTP client implementation
inline Client::Client(const char* host, int port)
    : host_(host)
    , port_(port)
{
}

struct RequestFunctor
{
    const Request& req;
    Response& res;
    RequestFunctor(const Request& req, Response& res):
        req(req), res(res) {
    }

    bool operator()(socket_t sock) {
        // Send request
        detail::write_request(sock, req);

        // Receive response
        if (!detail::read_response_line(sock, res) ||
            !detail::read_headers(sock, res.headers)) {
            return false;
        }
        if (req.method != "HEAD") {
            if (!detail::read_content(sock, res)) {
                return false;
            }
        }

        return true;
    }
};

inline bool Client::send(const Request& req, Response& res)
{
    int sock = detail::create_client_socket(host_.c_str(), port_);
    if (sock == -1) {
        return false;
    }

    RequestFunctor functor(req, res);
    return detail::read_and_close_socket(sock, functor);
}

inline Response* Client::get(const char* url)
{
    Request req;
    req.method = "GET";
    req.url = url;

    std::unique_ptr<Response> res(new Response);

    return send(req, *res) ? res.release() : nullptr;
}

inline Response* Client::head(const char* url)
{
    Request req;
    req.method = "HEAD";
    req.url = url;

    std::unique_ptr<Response> res(new Response);

    return send(req, *res) ? res.release() : nullptr;
}

inline Response* Client::post(
    const char* url, const std::string& body, const char* content_type)
{
    Request req;
    req.method = "POST";
    req.url = url;
    req.set_header("Content-Type", content_type);
    req.body = body;

    std::unique_ptr<Response> res(new Response);

    return send(req, *res) ? res.release() : nullptr;
}

inline Response* Client::post(
    const char* url, const void* body, size_t body_size, const char* content_type)
{
    Request req;
    req.method = "POST";
    req.url = url;
    req.set_header("Content-Type", content_type);
    const char* body_ptr = reinterpret_cast<const char*>(body);
    req.body.assign(body_ptr, body_ptr + body_size);

    std::unique_ptr<Response> res(new Response);

    return send(req, *res) ? res.release() : nullptr;
}


inline Response* Client::post(
    const char* url, const Map& params)
{
    std::string query;
    for (Map::const_iterator it = params.begin(); it != params.end(); ++it) {
        if (it != params.begin()) {
            query += "&";
        }
        query += it->first;
        query += "=";
        query += it->second;
    }

    return post(url, query, "application/x-www-form-urlencoded");
}

} // namespace httplib

#endif

// vim: et ts=4 sw=4 cin cino={1s ff=unix
