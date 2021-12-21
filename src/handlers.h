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

    http::request<http::string_body> add_user_request(User &);

    void add_user_response(http::response<http::string_body>, User &);

    http::request<http::string_body> login_user_request(User &);

    void login_user_response(http::response<http::string_body>, User &);

    http::request<http::string_body> find_friend_request(Friend &, QString mode);

    void find_friend_response(http::response<http::string_body>, Friend &);

    http::request<http::string_body> create_chat_request(Chat &);

    void create_chat_response(http::response<http::string_body>, Chat &);

    http::request<http::string_body> send_message_request(Message &);

    void send_message_response(http::response<http::string_body>, Message &);

    http::request<http::string_body> find_chat_by_id_request(Chat &);

    void find_chat_by_id_response(http::response<http::string_body>, Chat &);

    http::request<http::string_body> get_all_chat_msg_request(Chat &);

    std::vector<Message> get_all_chat_msg_response(http::response<http::string_body>);

    http::request<http::string_body> update_user_info_request(User &);

    std::vector<uint64_t> update_user_info_response(http::response<http::string_body>);

    http::request<http::string_body> update_current_chat_request(Chat, User);

    std::vector<Message> update_current_chat_response(http::response<http::string_body>);
};

#endif  // HANDLERS_H
