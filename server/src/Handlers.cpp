#include <iostream>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include "boost/filesystem/fstream.hpp"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

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
                     {"name", user.name},
                     {"login", user.login},
                     {"created_at", user.created_at},
                     {"profile_avatar", avatarBase64},
                     {"chats_id", json::value_from(user.chats_id)}
             }
            }
    };
    return json::serialize(responseValue);
}

std::string Handlers::GetChatBody(Chat chat) {
    json::value responseValue = {
            {"chat",
             {
                     {"chat_id", chat.id},
                     {"chat_name", chat.chat_name},
                     {"created_at", chat.created_at},
                     {"total_messages", chat.total_messages},
                     {"users_id", json::value_from(chat.users_id)}
             }
            }
    };
    return json::serialize(responseValue);
}

std::string Handlers::GetMessageBody(Message msg){
    std::string attachmentBase64;
    std::string attachmentBinary;
    if (!msg.attachment.empty()) {
        std::ifstream file(msg.attachment, std::ios::binary);
        if (file.good()) {
            file.seekg(0, std::ios::end);
            attachmentBinary.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(attachmentBinary.data(), attachmentBinary.size());
            file.close();

            size_t size = beast::detail::base64::encoded_size(
                    attachmentBinary.size());
            attachmentBase64.resize(size);
            beast::detail::base64::encode((void *) attachmentBase64.data(),
                                          attachmentBinary.data(),
                                          attachmentBinary.size());
        } else {
            attachmentBase64 = "";
        }
    } else {
        attachmentBase64 = "";
    }
    json::value responseValue = {
                     {"message_id", msg.id},
                     {"sender_id", msg.sender_id},
                     {"chat_id", msg.chat_id},
                     {"text", msg.text},
                     {"attachment", attachmentBase64},
                     {"created_at", msg.created_at}
    };
    return json::serialize(responseValue);
}

http::response<http::string_body>
Handlers::AddUser(http::request<http::string_body> request) {
    http::response<http::string_body> response;
    json::value jv = json::parse(request.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");

    auto name = json::value_to<std::string>(obj.at("name"));
    auto login = json::value_to<std::string>(obj.at("login"));
    auto password = json::value_to<std::string>(obj.at("password"));
    auto avatarBase64 = json::value_to<std::string>(obj.at("profile_avatar"));
    UserForm userForm;
    userForm.name = name;
    userForm.login = login;
    userForm.password = password;
    if (!avatarBase64.empty()) {
        userForm.profile_avatar = AVATARS + login + ".png";
    } else {
        userForm.profile_avatar = "";
    }

    auto user = database.AddUser(userForm);
    // Сохраняем картинку после добавления юзера
    if (!avatarBase64.empty()) {
        std::string avatarBinary;
        size_t size = beast::detail::base64::decoded_size(avatarBase64.size());
        avatarBinary.resize(size);
        beast::detail::base64::decode((void *) avatarBinary.data(),
                                      avatarBase64.data(),
                                      avatarBase64.size());
        std::ofstream file(userForm.profile_avatar);
        file.write(avatarBinary.data(), size);
    }

    std::string responseBody = GetUserBody(user);

    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length, std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;

    return response;
}

http::response<http::string_body>
Handlers::AddChat(http::request<http::string_body> request) {
    http::response<http::string_body> response;
    json::value jv = json::parse(request.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("chat");

    ChatForm chatForm;
    chatForm.chat_name = json::value_to<std::string>(obj.at("chat_name"));
    chatForm.users_id = json::value_to<std::vector<unsigned long>>(
            obj.at("users_id"));

    auto chat = database.AddChat(chatForm);

    std::string responseBody = GetChatBody(chat);

    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length, std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response <http::string_body>
Handlers::Authorization(http::request <http::string_body> request){
    http::response<http::string_body> response;
    json::value jv = json::parse(request.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");

    UserLogin userLogin;
    userLogin.login = json::value_to<std::string>(obj.at("login"));
    userLogin.password = json::value_to<std::string>(obj.at("password"));

    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "application/json");
    if (database.Authorization(userLogin)){
        auto user = database.GetUserByLogin(userLogin.login);
        std::string responseBody = GetUserBody(user);
        response.set(http::field::content_length, std::to_string(responseBody.size()));
        response.result(http::status::ok);
        response.body() = responseBody;
    }else{
        response.set(http::field::content_length, "0");
        response.result(http::status::unauthorized);
        response.body() = "";
    }

    return response;
}

http::response<http::string_body>
Handlers::AddMessage(http::request<http::string_body> request) {
    http::response<http::string_body> response;
    json::value jv = json::parse(request.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("message");

    MessageForm msgForm;
    msgForm.sender_id = json::value_to<unsigned long>(obj.at("sender_id"));
    msgForm.chat_id = json::value_to<unsigned long>(obj.at("chat_id"));
    msgForm.text = json::value_to<std::string>(obj.at("text"));
    auto attachmentBase64 = json::value_to<std::string>(obj.at("attachment"));

    if (!attachmentBase64.empty()) {
        boost::uuids::random_generator generator;
        boost::uuids::uuid uuid1 = generator();
        msgForm.attachment = IMAGES + to_string(uuid1) + ".png";
    } else {
        msgForm.attachment = "";
    }

    auto msg = database.AddMessage(msgForm);
    // Сохраняем картинку после добавления сообщения
    if (!attachmentBase64.empty()) {
        std::string attachmentBinary;
        size_t size = beast::detail::base64::decoded_size(attachmentBase64.size());
        attachmentBinary.resize(size);
        beast::detail::base64::decode((void *) attachmentBinary.data(),
                                      attachmentBase64.data(),
                                      attachmentBase64.size());
        std::ofstream file(msgForm.attachment);
        file.write(attachmentBinary.data(), size);
    }

    std::string responseBody = GetMessageBody(msg);
    responseBody = "{\"message\":" + responseBody + "}";

    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length, std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;

    return response;
}

http::response<http::string_body>
Handlers::GetUser(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto delimiterPos = tar.find('?');
    std::string params = tar.substr(delimiterPos + 1);
    auto name = params.substr(0, params.find('='));
    auto value = params.substr(params.find('=') + 1);
    User user;
    if (name == "id") {
        auto user_id = std::stoul(value);
        user = database.GetUserByID(user_id);
    } else if (name == "login") {
        user = database.GetUserByLogin(value);
    } else {
        throw InvalidInputs();
    }

    std::string responseBody = GetUserBody(user);

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length, std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetChat(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto delimiterPos = tar.find('?');
    std::string params = tar.substr(delimiterPos + 1);
    auto name = params.substr(0, params.find('='));
    auto value = params.substr(params.find('=') + 1);
    auto chat_id = std::stoul(value);
    auto chat = database.GetChatByID(chat_id);

    std::string responseBody = GetChatBody(chat);

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length, std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetChatMessages(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto delimiterPos = tar.find('?');
    std::string params = tar.substr(delimiterPos + 1);
    auto name = params.substr(0, params.find('='));
    auto value = params.substr(params.find('=') + 1);
    auto chat_id = std::stoul(value);
    auto msgs = database.GetChatMessages(chat_id);

    std::string responseBody = "{\"messages\": [";
    for (auto &msg : msgs){
        responseBody += GetMessageBody(msg) + ",";
    }
    responseBody.pop_back();
    responseBody += "]}";

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length, std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetMessage(http::request<http::string_body> request) {
    throw NotImplemented();
}