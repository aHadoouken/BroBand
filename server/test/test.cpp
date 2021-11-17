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
#include "Router.h"
#include "Server.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

TEST(SERVER_TEST, server) {

    Server s;
    EXPECT_THROW(s.Run(), NotImplemented);
}

TEST(CONNECTION_TEST, connection) {

    Connection conn;
    EXPECT_THROW(conn.Start(), NotImplemented);
}

TEST(HANDLERS_TEST, AddChat) {

    Handlers hand;
    http::request <http::string_body> request;

    EXPECT_THROW(hand.AddChat(request), NotImplemented);
}

TEST(HANDLERS_TEST, AddMessage) {

    Handlers hand;
    http::request <http::string_body> request;

    EXPECT_THROW(hand.AddMessage(request), NotImplemented);
}

TEST(HANDLERS_TEST, AddUser) {

    Handlers hand;
    http::request <http::string_body> request;

    EXPECT_THROW(hand.AddUser(request), NotImplemented);
}

TEST(HANDLERS_TEST, GetChat) {

    Handlers hand;
    http::request <http::string_body> request;

    EXPECT_THROW(hand.GetChat(request), NotImplemented);
}

TEST(HANDLERS_TEST, GetMessage) {

    Handlers hand;
    http::request <http::string_body> request;

    EXPECT_THROW(hand.GetMessage(request), NotImplemented);
}

TEST(HANDLERS_TEST, GetUser) {

    Handlers hand;
    http::request <http::string_body> request;

    EXPECT_THROW(hand.GetUser(request), NotImplemented);
}

TEST(ROUTING_TEST, AddHandler) {

    Handlers hand;

    Router<Handlers,
            http::response<http::string_body>,
            http::request <http::string_body>> router;

    http::request <http::string_body> req;

    router.AddHandler("GetUser", hand);

//    router.ProcessRoute("GetUser", req);
}

int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}