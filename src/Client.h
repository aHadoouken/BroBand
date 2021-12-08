#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <QString>

#include <boost/bind/bind.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <thread>
#include <boost/asio.hpp>


using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>


class Client {
   private:
    tcp::resolver resolver_;

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;

    http::request<http::string_body> request_;
    http::response<http::string_body> response_;
    const std::string server = "5c61-195-19-47-102.ngrok.io";
    const std::string port = "http";

    bool status;
    QString errors;

   private:
    void handle_resolve(beast::error_code, tcp::resolver::results_type);
    void handle_connect(beast::error_code, tcp::resolver::results_type::endpoint_type);
    void handle_write_request(beast::error_code, std::size_t);
    void handle_read(beast::error_code, std::size_t);

   public:
    explicit Client(net::io_context&);
    ~Client();
    void run(http::request<http::string_body>);
    void do_write(http::request<http::string_body> request);
    void do_close();
    http::response<http::string_body> get_response();
    bool get_status();

};

#endif  // CLIENT_H
