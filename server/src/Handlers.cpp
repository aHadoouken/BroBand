#include <iostream>
#include "Exceptions.h"
#include "Handlers.h"

http::response<http::string_body>
Handlers::AddUser(http::request<http::string_body> request) {
    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = request.body();
    return response;
}

http::response<http::string_body>
Handlers::AddChat(http::request<http::string_body> request) {
    throw NotImplemented();
}

http::response<http::string_body>
Handlers::AddMessage(http::request<http::string_body> request) {
    throw NotImplemented();
}

http::response<http::string_body>
Handlers::GetUser(http::request<http::string_body> request) {
    throw NotImplemented();
}

http::response<http::string_body>
Handlers::GetChat(http::request<http::string_body> request) {
    throw NotImplemented();
}

http::response<http::string_body>
Handlers::GetMessage(http::request<http::string_body> request) {
    throw NotImplemented();
}