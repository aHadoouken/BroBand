#include <gtest/gtest.h>
#include "Exceptions.h"

#include <boost/beast/core.hpp>
#include <boost/json.hpp>


#include "Exceptions.h"
#include "Handlers.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


TEST(HANDLERS_TEST, AddUser) {
    Handlers hand;
    http::request<http::string_body> request;
    request.body() = "{\"user\": \n"
                     "\t{\n"
                     "\t\"login\": \"login_test\",\n"
                     "\t\"name\": \"name_test\",\n"
                     "\t\"password\": \"password_test\",\n"
                     "\t\"profile_avatar\": \"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\"\n"
                     "\t}\n"
                     "}";
    http::response<http::string_body> response;
    EXPECT_NO_THROW(response = hand.AddUser(request));
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");
    auto login = json::value_to<std::string>(obj.at("login"));
    auto name = json::value_to<std::string>(obj.at("name"));
    EXPECT_EQ(login, "login_test");
    EXPECT_EQ(name, "name_test");
    EXPECT_TRUE((json::value_to<boost::json::array>(obj.at("chats_id"))).empty());
    EXPECT_EQ((int)response.result(), 200);
    request.body() = "{\"user\": \n"
                     "\t{\n"
                     "\t\"login\": \"login_test\",\n"
                     "\t\"name\": \"name_test\",\n"
                     "\t\"password\": \"password_test\",\n"
                     "\t\"profile_avatar\": \"\""
                     "\t}\n"
                     "}";
    EXPECT_THROW(response = hand.AddUser(request), pqxx::unique_violation);
}

TEST(HANDLERS_TEST, GetUser) {
    Handlers hand;
    http::request<http::string_body> request;
    http::response<http::string_body> response;
    request.target("/user_info?id=1");
    EXPECT_NO_THROW(response = hand.GetUser(request));
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");
    auto login = json::value_to<std::string>(obj.at("login"));
    auto name = json::value_to<std::string>(obj.at("name"));
    EXPECT_EQ(login, "user1");
    EXPECT_EQ(name, "name1");
    EXPECT_EQ(json::value_to<boost::json::array>(obj.at("chats_id")), json::array({1,2,3}));
    EXPECT_EQ((int)response.result(), 200);
}


int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}