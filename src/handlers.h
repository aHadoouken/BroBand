#ifndef HANDLERS_H
#define HANDLERS_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "Structs.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Handlers  {

public:
     Handlers();

    http::request<http::string_body>
    add_user_request(User &);

    void add_user_response(http::response<http::string_body> response, User &);

    http::request<http::string_body>
    login_user_request(User &);

    void login_user_response(http::response<http::string_body> response, User &);

    http::request<http::string_body>
    get_user_chat_request(User &);

    void get_user_chat_response(http::response<http::string_body> response, User &);

    http::request<http::string_body>
    find_friend_request(Friend &);

    void find_friend_response(http::response<http::string_body> response, Friend &);

    http::request<http::string_body>
    create_chat_request(Chat &);

    void create_chat_response(http::response<http::string_body> response, Chat &);

    http::request<http::string_body>
    send_message_request(Message &);

    void send_message_response(http::response<http::string_body> response, Message &);
};

#endif // HANDLERS_H
