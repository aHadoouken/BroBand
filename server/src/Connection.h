#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "db_implementation.h"
#include "Handlers.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(tcp::socket &&socket, Handlers &handlers);

    int Start();

private:
    void DoRead();

    void HandleRead(beast::error_code e,
                    std::size_t bytes_transferred);

    void HandleWrite(bool close,
                     beast::error_code e,
                     std::size_t bytes_transferred);

    void DoClose();

private:
    beast::tcp_stream stream_;

    Handlers &handlers_;

    beast::flat_buffer buffer_;

    http::request<http::string_body> request_;

    std::shared_ptr<void> res_;

    void processAddUser(const http::request<http::string_body> &req,
                        http::response<http::string_body> &res);
};