#include "Exceptions.h"
#include "Connection.h"
#include <boost/json/basic_parser.hpp>

Connection::Connection(tcp::socket &&socket, Handlers &handlers)
        : stream_(std::move(socket)), handlers_(handlers) {
    //parser.body_limit(1024*1024*10);
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
    // request_ = parser.release();
    std::cout << "Request target: " << request_.target() << std::endl;
    std::cout << "Request body: " << request_.body() << std::endl;

    if (!e) {
        http::response<http::string_body> res{http::status::bad_request,
                                              request_.version()};
        try {
            std::string req_tar = static_cast<std::string>(request_.target());
            auto delimiterPos = req_tar.find('?');
            auto target = req_tar.substr(0, delimiterPos);

            if (target == "/add_user" &&
                request_.method() == http::verb::post) {
                res = handlers_.AddUser(request_);
            } else if (target == "/add_chat" &&
                       request_.method() == http::verb::post) {
                res = handlers_.AddChat(request_);
            } else if (target == "/user_info" &&
                       request_.method() == http::verb::get) {
                res = handlers_.GetUser(request_);
            } else if (target == "/chat_info" &&
                       request_.method() == http::verb::get) {
                res = handlers_.GetChat(request_);
            } else if (target == "/user_logIn" &&
                       request_.method() == http::verb::post) {
                res = handlers_.Authorization(request_);
            } else if (target == "/add_message" &&
                       request_.method() == http::verb::post) {
                res = handlers_.AddMessage(request_);
            } else if (target == "/chat_messages" &&
                       request_.method() == http::verb::get) {
                res = handlers_.GetChatMessages(request_);
            } else {
                res.set(http::field::content_type, "application/json");
                res.result(http::status::not_found);
                res.body() = "Not found";
            }
        }
        catch (pqxx::unique_violation &ex) {
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.result(http::status::method_not_allowed);
            res.body() = ex.what();
        }
        catch (pqxx::plpgsql_no_data_found &ex) {
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.result(http::status::not_found);
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

        std::cout << "Response status: " << std::endl << res.reason()<< std::endl;
        std::cout << "Response body: " << std::endl << res.body() << std::endl;
        std::cout << "======================================================="<< std::endl;

        auto sp = std::make_shared<http::message<false, http::string_body>>(
                std::move(res));
        res_ = sp;

        http::async_write(stream_,
                          *sp,
                          beast::bind_front_handler(
                                  &Connection::HandleWrite,
                                  shared_from_this(),
                                  sp->need_eof()));
    } else {
        std::cout << "Error: " << e << "\n";
        return DoClose();
    }
}