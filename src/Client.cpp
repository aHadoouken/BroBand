#include "Client.h"

Client::Client(net::io_context& ioc) : resolver_(net::make_strand(ioc)), stream_(net::make_strand(ioc)) {
    status = false;
}

Client::~Client() = default;

void Client::run(http::request<http::string_body> request) {
    request.set(http::field::host, server);
    request_ = request;
    resolver_.async_resolve(server, port, beast::bind_front_handler(&Client::handle_resolve, this));
}

void Client::handle_resolve(beast::error_code err, tcp::resolver::results_type results) {
    if (!err) {
        stream_.async_connect(results, beast::bind_front_handler(&Client::handle_connect, this));
        status = true;
    } else {
        status = false;
        errors = "Error in handle_resolve: " + QString::fromStdString(err.message());
        std::cout << errors.toStdString() << std::endl;
    }
}

void Client::handle_connect(beast::error_code err, const tcp::resolver::results_type::endpoint_type&) {
    if (!err) {
        http::async_write(stream_, request_, beast::bind_front_handler(&Client::handle_write_request, this));
        status = true;
    } else {
        status = false;
        errors = "Error in handle_connect: " + QString::fromStdString(err.message());
        std::cout << errors.toStdString() << std::endl;
    }
}

void Client::handle_write_request(beast::error_code err, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (!err) {
        response_ = {};
        http::async_read(stream_, buffer_, response_, beast::bind_front_handler(&Client::handle_read, this));
        status = true;
    } else {
        status = false;
        errors = "Error in handle_write_request: " + QString::fromStdString(err.message());
        std::cout << errors.toStdString() << std::endl;
    }
}

void Client::do_write(http::request<http::string_body> request) {
    request.set(http::field::host, server);
    request_ = request;
    http::async_write(stream_, request_, beast::bind_front_handler(&Client::handle_write_request, this));
}

void Client::handle_read(beast::error_code err, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (!err) {
        status = true;
    } else {
        status = false;
        errors = "Error in handle_read: " + QString::fromStdString(err.message());
        std::cout << errors.toStdString() << std::endl;
    }
}

void Client::do_close() {
    if (!status) {
        return;
    }
    beast::error_code err;
    stream_.socket().shutdown(tcp::socket::shutdown_both, err);
}

bool Client::get_status() const { return status; }

http::response<http::string_body> Client::get_response() const { return response_; }
