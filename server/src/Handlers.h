#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "db_implementation.h"

#define AVATARS "/home/alex/Avatars/"
#define IMAGES "/home/alex/Images/"
#define DB_CONFIG "/home/alex/mail_cpp/Boost_test/db_config.cfg"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Handlers {
public:
    Handlers (): database(DB_CONFIG) {};

    http::response <http::string_body>
    AddUser(http::request <http::string_body> request);

    http::response <http::string_body>
    AddChat(http::request <http::string_body> request);

    http::response <http::string_body>
    AddMessage(http::request <http::string_body> request);

    http::response <http::string_body>
    GetUser(http::request <http::string_body> request);

    http::response <http::string_body>
    GetChat(http::request <http::string_body> request);

    http::response <http::string_body>
    GetMessage(http::request <http::string_body> request);

private:
    PostgresDB database;

    std::string GetUserBody(User user);
};
