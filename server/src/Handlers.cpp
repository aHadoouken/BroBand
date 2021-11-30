#include <iostream>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include "boost/filesystem/fstream.hpp"

#include "Exceptions.h"
#include "Handlers.h"
#include "structs.h"

namespace json = boost::json;
namespace filesystem = boost::filesystem;

std::string Handlers::GetUserBody(User user) {
    std::string avatarBase64;
    std::string avatarBinary;
    if (!user.profile_avatar.empty()) {
        std::ifstream file(user.profile_avatar, std::ios::binary);
        if (file.good()) {
            file.seekg(0, std::ios::end);
            avatarBinary.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(avatarBinary.data(), avatarBinary.size());
            file.close();

            size_t size = beast::detail::base64::encoded_size(
                    avatarBinary.size());
            avatarBase64.resize(size);
            beast::detail::base64::encode((void *) avatarBase64.data(),
                                          avatarBinary.data(),
                                          avatarBinary.size());
        } else {
            avatarBase64 = "";
        }
    } else {
        avatarBase64 = "";
    }
    json::value responseValue = {
            {"user",
             {
                     {"user_id", user.id},
                     {"nickname", user.nickname},
                     {"created_at", user.created_at},
                     {"profile_avatar", avatarBase64},
                     {"chats_id", json::value_from(user.chats_id)}
             }
            }
    };
    return json::serialize(responseValue);
}

http::response<http::string_body>
Handlers::AddUser(http::request<http::string_body> request) {
    http::response<http::string_body> response;
    json::value jv = json::parse(request.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");

    auto nickname = json::value_to<std::string>(obj.at("nickname"));
    auto avatarBase64 = json::value_to<std::string>(obj.at("profile_avatar"));
    UserForm userForm;
    userForm.nickname = nickname;
    if (!avatarBase64.empty()) {
        std::string avatarBinary;
        size_t size = beast::detail::base64::decoded_size(avatarBase64.size());
        avatarBinary.resize(size);
        beast::detail::base64::decode((void *) avatarBinary.data(),
                                      avatarBase64.data(),
                                      avatarBase64.size());
        std::string ofilePath = AVATARS + nickname + ".png";
        std::ofstream file(ofilePath);
        file.write(avatarBinary.data(), size);

        userForm.profile_avatar = ofilePath;
    } else {
        userForm.profile_avatar = "";
    }

    auto user = database.AddUser(userForm);

    std::string responseBody = GetUserBody(user);

    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
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