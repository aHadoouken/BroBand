//
// Created by d3vyatk4 on 17.11.2021.
//

#include <gtest/gtest.h>
#include "Exceptions.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "Connection.h"
#include "Exceptions.h"
#include "Handlers.h"
#include "Server.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

TEST(HANDLERS_TEST, AddChat) {

    Handlers hand;
    http::request<http::string_body> request;
    http::response <http::string_body> response_real = hand.AddChat(request);


    EXPECT_EQ((int)response_real.result(), 200);
    EXPECT_EQ(response_real.body(), "");
//    EXPECT_EQ(response_real.method_string(), "POST");                               //TODO: нужно выставить флаг is_request
                                                                                      // https://www.boost.org/doc/libs/develop/libs/beast/doc/html/beast/ref/boost__beast__http__response.html

}

TEST(HANDLERS_TEST, AddMessage) {

    Handlers hand;
    http::request<http::string_body> request;
    http::response <http::string_body> response_real = hand.AddMessage(request);

    EXPECT_EQ((int)response_real.result(), 200);
    EXPECT_EQ(response_real.body(), "");
}

TEST(HANDLERS_TEST, AddUser) {

    Handlers hand;
    http::request<http::string_body> request;
    http::response <http::string_body> response_real = hand.AddUser(request);

    EXPECT_EQ((int)response_real.result(), 200);
    EXPECT_EQ(response_real.body(), "");
}

TEST(HANDLERS_TEST, GetChat) {

    Handlers hand;
    http::request<http::string_body> request;
    http::response <http::string_body> response_real = hand.GetChat(request);

    EXPECT_EQ((int)response_real.result(), 200);
    EXPECT_EQ(response_real.body(), "");
}

TEST(HANDLERS_TEST, GetMessage) {

    Handlers hand;
    http::request<http::string_body> request;
    http::response <http::string_body> response_real = hand.GetMessage(request);

    EXPECT_EQ((int)response_real.result(), 200);
    EXPECT_EQ(response_real.body(), "");
}

TEST(HANDLERS_TEST, GetUser) {

    Handlers hand;
    http::request<http::string_body> request;
    http::response <http::string_body> response_real = hand.GetUser(request);

    EXPECT_EQ((int)response_real.result(), 200);
    EXPECT_EQ(response_real.body(), "");
}


int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}