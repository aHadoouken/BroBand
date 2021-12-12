#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <torch/script.h>

#include "db_implementation.h"
#include "detecting.h"

#define AVATARS "/home/alex/mail_cpp/Avatars/"
#define IMAGES "/home/alex/mail_cpp/Images/"
#define DB_CONFIG "/home/alex/mail_cpp/Boost_test/db_config.cfg"
#define MODEL "../server/model/annotation_resnet34_porn_model.pt"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Handlers {
public:
    Handlers (): database(DB_CONFIG) {
        pornDetector.load_model(MODEL);
    };

    http::response <http::string_body>
    AddUser(http::request <http::string_body> request);

    http::response <http::string_body>
    AddChat(http::request <http::string_body> request);

    http::response <http::string_body>
    Authorization(http::request <http::string_body> request);

    http::response <http::string_body>
    AddMessage(http::request <http::string_body> request);

    http::response <http::string_body>
    GetUser(http::request <http::string_body> request);

    http::response <http::string_body>
    GetChat(http::request <http::string_body> request);

    http::response <http::string_body>
    GetChatMessages(http::request <http::string_body> request);

    http::response <http::string_body>
    GetChatMessagesAfterID(http::request <http::string_body> request);

    http::response <http::string_body>
    GetMessage(http::request <http::string_body> request);

private:
    PostgresDB database;

    PornImageDetector pornDetector;

    std::string GetUserBody(User user);

    std::string GetChatBody(Chat chat);

    std::string GetMessageBody(Message msg);

    bool GetMd5(std::string &str_md5, const char * const buffer, size_t buffer_size);

    std::map<std::string, std::string> ParseParams(const std::string& url);
};
