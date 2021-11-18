#include "Client.h"

void fail(beast::error_code ec, char const* what) { std::cerr << what << ": " << ec.message() << "\n"; }

Client::Client(net::io_context& ioc) : resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc)) {}

void Client::run(char const* host, char const* port, char const* text) {
    host_ = host;
    text_ = text;
    resolver_.async_resolve(host, port, beast::bind_front_handler(&Client::onResolve, shared_from_this()));
}

void Client::onResolve(beast::error_code ec, tcp::resolver::results_type results) {
    if (ec) return fail(ec, "resolve");

    beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

    beast::get_lowest_layer(ws_).async_connect(
        results, beast::bind_front_handler(&Client::onConnect, shared_from_this()));
}

void Client::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
    if (ec) return fail(ec, "connect");

    beast::get_lowest_layer(ws_).expires_never();

    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));

    ws_.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
        req.set(http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
    }));

    ws_.async_handshake(host_, "/", beast::bind_front_handler(&Client::onHandshake, shared_from_this()));
}

void Client::onHandshake(beast::error_code ec) {
    if (ec) return fail(ec, "handshake");

    ws_.async_write(net::buffer(text_), beast::bind_front_handler(&Client::onWrite, shared_from_this()));
}

void Client::onWrite(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "write");

    ws_.async_read(buffer_, beast::bind_front_handler(&Client::onRead, shared_from_this()));
}

void Client::onRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) return fail(ec, "read");

    ws_.async_close(websocket::close_code::normal,
                    beast::bind_front_handler(&Client::onClose, shared_from_this()));
}

void Client::onClose(beast::error_code ec) {
    if (ec) return fail(ec, "close");

    std::cout << beast::make_printable(buffer_.data()) << std::endl;
}
