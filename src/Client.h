#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "Chat.h"
#include "Message.h"
#include "User.h"

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

class Client : public std::enable_shared_from_this<Client> {
   private:
    tcp::resolver resolver_;
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    std::string host_;
    std::string text_;

    User *user;
    Chat *chat;
    Message *msg;

   public:
    explicit Client(net::io_context &);
    ~Client();
    void run(char const *, char const *, char const *);

   private:
    void onResolve(beast::error_code, tcp::resolver::results_type);
    void onConnect(beast::error_code, tcp::resolver::results_type::endpoint_type);
    void onHandshake(beast::error_code);
    void onWrite(beast::error_code, std::size_t);
    void onRead(beast::error_code, std::size_t);
    void onClose(beast::error_code);
};

#endif  // CLIENT_H
