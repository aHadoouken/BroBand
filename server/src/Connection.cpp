#include "Exceptions.h"
#include "Connection.h"

Connection::Connection(tcp::socket &&socket, Handlers &handlers)
        : stream_(std::move(socket)), handlers_(handlers) {
}

int Connection::Start() {
    net::dispatch(stream_.get_executor(),
                  beast::bind_front_handler(&Connection::DoRead,
                                            shared_from_this()));
}

void Connection::DoRead() {
    request_ = {};

    http::async_read(stream_, buffer_, request_,
                     beast::bind_front_handler(
                             &Connection::HandleRead,
                             shared_from_this()));
}

void Connection::HandleWrite(bool close,
                             beast::error_code e,
                             std::size_t bytes_transferred) {
    if (close) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return DoClose();
    }

    if (!e) {
        DoRead();
    }
}

void Connection::DoClose() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}

void Connection::HandleRead(beast::error_code e,
                            std::size_t bytes_transferred) {
    if (e == http::error::end_of_stream)
        return DoClose();

    if (!e) {
        http::response<http::string_body> res{http::status::bad_request,
                                              request_.version()};

        if (request_.target() == "/add_user" &&
            request_.method() == http::verb::post) {
            processAddUser(request_, res);
        } else if (request_.target() == "/add_chat") {
            res = handlers_.AddChat(request_);
        } else {
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.result(http::status::not_found);
            res.body() = "Not found";
        }

        auto sp = std::make_shared<http::message<false, http::string_body>>(
                std::move(res));
        res_ = sp;

        http::async_write(stream_,
                          *sp,
                          beast::bind_front_handler(
                                  &Connection::HandleWrite,
                                  shared_from_this(),
                                  sp->need_eof()));
    }
}

void Connection::processAddUser(const http::request<http::string_body> &req,
                                http::response<http::string_body> &res) {
    try {
        res = handlers_.AddUser(req);
    }
    catch (pqxx::unique_violation &ex) {
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.result(http::status::method_not_allowed);
        res.body() = ex.what();
    }
    catch (pqxx::failure &ex) {
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.result(http::status::service_unavailable);
        res.body() = ex.what();
    }
    catch (std::exception &ex) {
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = ex.what();
    }
}