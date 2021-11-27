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

#include <boost/asio.hpp>

#include "Handlers.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


class Server : public std::enable_shared_from_this<Server> {
public:
    explicit Server(const std::string &address, const std::string &port,
                    std::size_t thread_pool_size);

    void Run();

private:
    void StartAccept();

    void HandleAccept(beast::error_code e, tcp::socket socket);

    void HandleStop();

private:
    Handlers handlers;

    net::io_context io_context_;
    tcp::acceptor acceptor_;

    /// The number of threads that will call io_context::run().
    std::size_t thread_pool_size_;

};