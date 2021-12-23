#ifndef HANDLERS_H
#define HANDLERS_H

#include <QObject>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>

#include "Structs.h"

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

class Handlers {
   public:
    Handlers();

    static http::request<http::string_body> add_user_request(User &);

    static void add_user_response(http::response<http::string_body>, User &);

    static http::request<http::string_body> login_user_request(User &);

    static void login_user_response(http::response<http::string_body>, User &);

    static http::request<http::string_body> find_friend_request(Friend &, QString mode);

    static void find_friend_response(http::response<http::string_body>, Friend &);

    static http::request<http::string_body> create_chat_request(Chat &);

    static void create_chat_response(http::response<http::string_body>, Chat &);

    static http::request<http::string_body> send_message_request(Message &);

    static void send_message_response(http::response<http::string_body>, Message &);

    static http::request<http::string_body> find_chat_by_id_request(Chat &);

    static void find_chat_by_id_response(http::response<http::string_body>, Chat &);

    static http::request<http::string_body> get_all_chat_msg_request(Chat &);

    static std::vector<Message> get_all_chat_msg_response(http::response<http::string_body>);

    static http::request<http::string_body> update_user_info_request(User &);

    static std::vector<uint64_t> update_user_info_response(http::response<http::string_body>);

    static http::request<http::string_body> update_current_chat_request(Chat, User);

    static std::vector<Message> update_current_chat_response(http::response<http::string_body>);
};

#endif  // HANDLERS_H
