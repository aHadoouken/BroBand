#include <iostream>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include "boost/filesystem/fstream.hpp"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <map>

#include "Exceptions.h"
#include "Handlers.h"
#include "structs.h"

namespace json = boost::json;
namespace filesystem = boost::filesystem;

bool Handlers::GetMd5(std::string &str_md5, const char *const buffer,
                      size_t buffer_size) {
    if (buffer == nullptr) {
        return false;
    }

    boost::uuids::detail::md5 boost_md5;
    boost_md5.process_bytes(buffer, buffer_size);
    boost::uuids::detail::md5::digest_type digest;
    boost_md5.get_digest(digest);
    const auto char_digest = reinterpret_cast<const char *>(&digest);
    str_md5.clear();
    boost::algorithm::hex(char_digest, char_digest +
                                       sizeof(boost::uuids::detail::md5::digest_type),
                          std::back_inserter(str_md5));
    return true;
}

std::map<std::string, std::string>
Handlers::ParseParams(const std::string &url) {
    std::map<std::string, std::string> map;
    auto delimiterPos = url.find('?');
    if (delimiterPos == std::string::npos)
        throw InvalidInputs("Unable to find parameters");
    std::string params = url.substr(delimiterPos + 1);
    std::string single_param, name, value;
    while (!params.empty()) {
        delimiterPos = params.find('&');
        if (delimiterPos != std::string::npos) {
            single_param = params.substr(0, delimiterPos);
            params = params.substr(delimiterPos + 1);
        } else {
            single_param = params;
            params = "";
        }
        delimiterPos = single_param.find('=');
        if (delimiterPos == std::string::npos)
            throw InvalidInputs("Unable to find parameters");
        name = single_param.substr(0, delimiterPos);
        value = single_param.substr(delimiterPos + 1);
        map[name] = value;
    }
    return map;
}

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
            {"chat_id",        chat.id},
            {"chat_name",      chat.chat_name},
            {"created_at",     chat.created_at},
            {"total_messages", chat.total_messages},
            {"users_id",       json::value_from(chat.users_id)}
    };
    return json::serialize(responseValue);
}

std::string Handlers::GetMessageBody(Message msg) {
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
            {"message_id",  msg.id},
            {"sender_id",   msg.sender_id},
            {"sender_name", msg.sender_name},
            {"chat_id",     msg.chat_id},
            {"chat_name",   msg.chat_name},
            {"text",        msg.text},
            {"attachment",  attachmentBase64},
            {"created_at",  msg.created_at}
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
    std::string password_hashed;
    if (!GetMd5(password_hashed, password.c_str(), password.length())) {
        throw InvalidInputs("Unable to hash password");
    }
    UserForm userForm;
    userForm.name = name;
    userForm.login = login;
    userForm.password = password_hashed;
    if (!avatarBase64.empty()) {
        userForm.profile_avatar = AVATARS + login + ".jpg";
    } else {
        userForm.profile_avatar = "";
    }

    auto user = database.AddUser(userForm);
    // Сохраняем картинку после добавления юзера
    if (!avatarBase64.empty()) {
        cv::Mat img = pornDetector.load_img(avatarBase64);
        // предобработка
        torch::Tensor img_tensor = pornDetector.preproccesing(img);
        // прогоняем по сетке изображение
        Probability probability = pornDetector.forward(img_tensor);
        std::cout << probability;
        // блюрим при необходимости
        avatarBase64 = pornDetector.blurring();
        //Сохраняем
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
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
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
    chatForm.users_id = json::value_to<std::vector<unsigned long >>(
            obj.at("users_id"));

    auto chat = database.AddChat(chatForm);

    std::string responseBody = GetChatBody(chat);
    responseBody = "{\"chat\":" + responseBody + "}";

    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::Authorization(http::request<http::string_body> request) {
    http::response<http::string_body> response;
    json::value jv = json::parse(request.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");

    auto login = json::value_to<std::string>(obj.at("login"));
    auto password = json::value_to<std::string>(obj.at("password"));
    std::string passwordHashedDB = database.GetPassword(login);
    std::string passwordHashed;
    if (!GetMd5(passwordHashed, password.c_str(), password.length())) {
        throw InvalidInputs("Unable to hash password");
    }
    if (passwordHashed == passwordHashedDB) {
        auto user = database.GetUserByLogin(login);
        std::string responseBody = GetUserBody(user);
        response.set(http::field::content_length,
                     std::to_string(responseBody.size()));
        response.result(http::status::ok);
        response.body() = responseBody;
    } else {
        response.set(http::field::content_length, "0");
        response.result(http::status::unauthorized);
        response.body() = "";
    }
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "application/json");

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
        msgForm.attachment = IMAGES + to_string(uuid1) + ".jpg";
    } else {
        msgForm.attachment = "";
    }

    auto msg = database.AddMessage(msgForm);
    // Сохраняем картинку после добавления сообщения
    if (!attachmentBase64.empty()) {
        cv::Mat img = pornDetector.load_img(attachmentBase64);
        // предобработка
        torch::Tensor img_tensor = pornDetector.preproccesing(img);
        // прогоняем по сетке изображение
        Probability probability = pornDetector.forward(img_tensor);
        std::cout << probability;
        // блюрим при необходимости
        attachmentBase64 = pornDetector.blurring();
        //Сохраняем
        std::string attachmentBinary;
        size_t size = beast::detail::base64::decoded_size(
                attachmentBase64.size());
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
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;

    return response;
}

http::response<http::string_body>
Handlers::GetUser(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto params = ParseParams(tar);
    User user;
    if (params.count("id")) {
        auto user_id = std::stoul(params["id"]);
        user = database.GetUserByID(user_id);
    } else if (params.count("login")) {
        user = database.GetUserByLogin(params["login"]);
    } else {
        throw InvalidInputs();
    }

    std::string responseBody = GetUserBody(user);

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetChat(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto params = ParseParams(tar);
    auto chat = database.GetChatByID(std::stoul(params["id"]));

    std::string responseBody = GetChatBody(chat);
    responseBody = "{\"chat\":" + responseBody + "}";

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetChatMessages(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto params = ParseParams(tar);
    auto msgs = database.GetChatMessages(std::stoul(params["chat_id"]));

    std::string responseBody = "{\"messages\": [";
    for (auto &msg: msgs) {
        responseBody += GetMessageBody(msg) + ",";
    }
    responseBody.pop_back();
    responseBody += "]}";

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetChatMessagesAfterID(http::request<http::string_body> request) {
    std::string tar = static_cast<std::string>(request.target());
    auto params = ParseParams(tar);
    auto msgs = database.GetChatMessagesAfterID(std::stoul(params["chat_id"]),
                                                std::stoul(params["message_id"]));

    std::string responseBody = "{\"messages\": [";
    for (auto &msg: msgs) {
        responseBody += GetMessageBody(msg) + ",";
    }
    responseBody.pop_back();
    responseBody += "]}";

    http::response<http::string_body> response;
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_length,
                 std::to_string(responseBody.size()));
    response.set(http::field::content_type, "application/json");
    response.result(http::status::ok);
    response.body() = responseBody;
    return response;
}

http::response<http::string_body>
Handlers::GetMessage(http::request<http::string_body> request) {
    throw NotImplemented();
}