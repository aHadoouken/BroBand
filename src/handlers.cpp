#include "handlers.h"

#include <QDir>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/json.hpp>
#include <boost/json/src.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

#include "Structs.h"
#include "boost/filesystem/fstream.hpp"

namespace json = boost::json;
namespace filesystem = boost::filesystem;

Handlers::Handlers() {}

http::request<http::string_body> Handlers::add_user_request(User &user) {
    http::request<http::string_body> request;
    std::string avatar_base64;
    std::string avatar_binary;
    if (!user.profile_avatar.isEmpty()) {
        std::ifstream file(user.profile_avatar.toStdString(), std::ios::binary);
        if (file.good()) {
            file.seekg(0, std::ios::end);
            avatar_binary.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(avatar_binary.data(), avatar_binary.size());
            file.close();

            size_t size = beast::detail::base64::encoded_size(avatar_binary.size());
            avatar_base64.resize(size);
            beast::detail::base64::encode((void *)avatar_base64.data(), avatar_binary.data(),
                                          avatar_binary.size());
        } else {
            avatar_base64 = "";
        }
    } else {
        avatar_base64 = "";
    }
    json::value request_value = {{"user",
                                  {{"name", user.name.toStdString()},
                                   {"login", user.login.toStdString()},
                                   {"password", user.password.toStdString()},
                                   {"profile_avatar", avatar_base64}}}};
    std::string request_body = json::serialize(request_value);
    request.version(11);
    request.method(http::verb::post);
    request.target("/add_user");
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::content_length, std::to_string(request_body.size()));
    request.set(http::field::content_type, "application/json");
    request.body() = request_body;
    return request;
}

void Handlers::add_user_response(http::response<http::string_body> response, User &user) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");
    user.id = json::value_to<uint64_t>(obj.at("user_id"));
    user.name = QString::fromStdString(json::value_to<std::string>(obj.at("name")));
    user.created_at = QString::fromStdString(json::value_to<std::string>(obj.at("created_at")));
    user.chats_id = json::value_to<std::vector<uint64_t>>(obj.at("chats_id"));
    user.profile_avatar = QString::fromStdString(json::value_to<std::string>(obj.at("profile_avatar")));
}

http::request<http::string_body> Handlers::login_user_request(User &user) {
    http::request<http::string_body> request;
    json::value request_value = {
        {"user", {{"login", user.login.toStdString()}, {"password", user.password.toStdString()}}}};
    std::string request_body = json::serialize(request_value);
    request.version(11);
    request.method(http::verb::post);
    request.target("/user_logIn");
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::content_length, std::to_string(request_body.size()));
    request.set(http::field::content_type, "application/json");
    request.body() = request_body;
    return request;
}

void Handlers::login_user_response(http::response<http::string_body> response, User &user) {
    std::cout << response << std::endl;
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");
    user.id = json::value_to<uint64_t>(obj.at("user_id"));
    user.name = QString::fromStdString(json::value_to<std::string>(obj.at("name")));
    user.created_at = QString::fromStdString(json::value_to<std::string>(obj.at("created_at")));
    user.chats_id = json::value_to<std::vector<uint64_t>>(obj.at("chats_id"));
    auto avatar = json::value_to<std::string>(obj.at("profile_avatar"));
    if (avatar.empty()) {
        user.profile_avatar = "";
        return;
    }
    QDir dir;
    if (!dir.exists("Images")) {
        if (!dir.mkdir("Images")) {
            user.profile_avatar = "";
            return;
        }
    }
    user.profile_avatar = dir.currentPath() + "/Images/" + user.login + ".jpg";
    std::string avatarBinary;
    size_t size = beast::detail::base64::decoded_size(avatar.size());
    avatarBinary.resize(size);
    beast::detail::base64::decode((void *)avatarBinary.data(), avatar.data(), avatar.size());
    std::ofstream file(user.profile_avatar.toStdString());
    file.write(avatarBinary.data(), size);
}

http::request<http::string_body> Handlers::find_friend_request(Friend &new_friend, QString mode) {
    http::request<http::string_body> request;
    request.version(11);
    request.method(http::verb::get);
    if (mode == "by_login") {
        request.target("/user_info?login=" + new_friend.login.toStdString());
    } else if (mode == "by_id") {
        request.target("/user_info?id=" + std::to_string(new_friend.id));
    }
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.body() = "";
    return request;
}

void Handlers::find_friend_response(http::response<http::string_body> response, Friend &new_friend) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");
    new_friend.id = json::value_to<uint64_t>(obj.at("user_id"));
    new_friend.name = QString::fromStdString(json::value_to<std::string>(obj.at("name")));
    new_friend.login = QString::fromStdString(json::value_to<std::string>(obj.at("login")));
    new_friend.created_at = QString::fromStdString(json::value_to<std::string>(obj.at("created_at")));
    auto avatar = json::value_to<std::string>(obj.at("profile_avatar"));
    if (avatar.empty()) {
        new_friend.profile_avatar = "";
        return;
    }
    QDir dir;
    if (!dir.exists("Images")) {
        if (!dir.mkdir("Images")) {
            new_friend.profile_avatar = "";
            return;
        }
    }
    new_friend.profile_avatar = dir.currentPath() + "/Images/" + new_friend.login + ".png";
    std::string avatarBinary;
    size_t size = beast::detail::base64::decoded_size(avatar.size());
    avatarBinary.resize(size);
    beast::detail::base64::decode((void *)avatarBinary.data(), avatar.data(), avatar.size());
    std::ofstream file(new_friend.profile_avatar.toStdString());
    file.write(avatarBinary.data(), size);
}

http::request<http::string_body> Handlers::get_all_chat_msg_request(Chat &chat) {
    http::request<http::string_body> request;
    request.version(11);
    request.method(http::verb::get);
    request.target("/chat_messages?chat_id=" + std::to_string(chat.id));
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.body() = "";
    return request;
}

std::vector<Message> Handlers::get_all_chat_msg_response(http::response<http::string_body> response) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("messages");
    std::vector<Message> msg;
    for (size_t i = 0; i < jv.as_array().size(); ++i) {
        Message new_msg;
        json::object const &obj = jv.as_array().at(i).as_object();
        new_msg.id = json::value_to<uint64_t>(obj.at("message_id"));
        new_msg.senderId = json::value_to<uint64_t>(obj.at("sender_id"));
        new_msg.sender_name = QString::fromStdString(json::value_to<std::string>(obj.at("sender_name")));
        new_msg.chatId = json::value_to<uint64_t>(obj.at("chat_id"));
        new_msg.chat_name = QString::fromStdString(json::value_to<std::string>(obj.at("chat_name")));
        new_msg.text = QString::fromStdString(json::value_to<std::string>(obj.at("text")));
        auto img = json::value_to<std::string>(obj.at("attachment"));
        if (img.empty()) {
            new_msg.hasAttachment = false;
            new_msg.attachment = "";
            msg.push_back(new_msg);
            continue;
        }
        QDir dir;
        if (!dir.exists("Images")) {
            if (!dir.mkdir("Images")) {
                new_msg.hasAttachment = false;
                new_msg.attachment = "";
                msg.push_back(new_msg);
                continue;
            }
        }
        boost::uuids::random_generator generator;
        boost::uuids::uuid uuid1 = generator();
        new_msg.hasAttachment = true;
        new_msg.attachment =
            dir.currentPath() + "/Images/" + QString::fromStdString(to_string(uuid1)) + ".png";
        std::string avatarBinary;
        size_t size = beast::detail::base64::decoded_size(img.size());
        avatarBinary.resize(size);
        beast::detail::base64::decode((void *)avatarBinary.data(), img.data(), img.size());
        std::ofstream file(new_msg.attachment.toStdString());
        file.write(avatarBinary.data(), size);
        msg.push_back(new_msg);
    }
    return msg;
}

http::request<http::string_body> Handlers::update_user_info_request(User &user) {
    http::request<http::string_body> request;
    request.version(11);
    request.method(http::verb::get);
    request.target("/user_info?id=" + std::to_string(user.id));
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.body() = "";
    return request;
}

std::vector<uint64_t> Handlers::update_user_info_response(http::response<http::string_body> response) {
    std::vector<uint64_t> chats_id;
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("user");
    chats_id = json::value_to<std::vector<uint64_t>>(obj.at("chats_id"));
    return chats_id;
}

http::request<http::string_body> Handlers::update_current_chat_request(Chat chat, User user) {
    http::request<http::string_body> request;
    request.version(11);
    request.method(http::verb::get);
    uint64_t current_chat_last_msg = user.user_chats_by_id.find(chat.id)->second.last_msg_id;
    request.target("/chat_last_messages?chat_id=" + std::to_string(chat.id) +
                   "&message_id=" + std::to_string(current_chat_last_msg));
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.body() = "";
    return request;
}

std::vector<Message> Handlers::update_current_chat_response(http::response<http::string_body> response) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("messages");
    std::vector<Message> msg;
    std::cout << response.body() << std::endl;
    for (size_t i = 0; i < jv.as_array().size(); ++i) {
        Message new_msg;
        json::object const &obj = jv.as_array().at(i).as_object();
        new_msg.id = json::value_to<uint64_t>(obj.at("message_id"));
        new_msg.senderId = json::value_to<uint64_t>(obj.at("sender_id"));
        new_msg.sender_name = QString::fromStdString(json::value_to<std::string>(obj.at("sender_name")));
        new_msg.chatId = json::value_to<uint64_t>(obj.at("chat_id"));
        new_msg.chat_name = QString::fromStdString(json::value_to<std::string>(obj.at("chat_name")));
        new_msg.text = QString::fromStdString(json::value_to<std::string>(obj.at("text")));
        auto img = json::value_to<std::string>(obj.at("attachment"));
        if (img.empty()) {
            new_msg.hasAttachment = false;
            new_msg.attachment = "";
            msg.push_back(new_msg);
            continue;
        }
        QDir dir;
        if (!dir.exists("Images")) {
            if (!dir.mkdir("Images")) {
                new_msg.hasAttachment = false;
                new_msg.attachment = "";
                msg.push_back(new_msg);
                continue;
            }
        }
        boost::uuids::random_generator generator;
        boost::uuids::uuid uuid1 = generator();
        new_msg.hasAttachment = true;
        new_msg.attachment =
            dir.currentPath() + "/Images/" + QString::fromStdString(to_string(uuid1)) + ".png";
        std::string avatarBinary;
        size_t size = beast::detail::base64::decoded_size(img.size());
        avatarBinary.resize(size);
        beast::detail::base64::decode((void *)avatarBinary.data(), img.data(), img.size());
        std::ofstream file(new_msg.attachment.toStdString());
        file.write(avatarBinary.data(), size);
        msg.push_back(new_msg);
    }
    return msg;
}

http::request<http::string_body> Handlers::create_chat_request(Chat &chat) {
    http::request<http::string_body> request;
    json::value request_value = {
        {"chat", {{"chat_name", chat.name.toStdString()}, {"users_id", chat.users_id}}}};
    std::string request_body = json::serialize(request_value);
    request.version(11);
    request.method(http::verb::post);
    request.target("/add_chat");
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::content_length, std::to_string(request_body.size()));
    request.set(http::field::content_type, "application/json");
    request.body() = request_body;
    return request;
}

void Handlers::create_chat_response(http::response<http::string_body> response, Chat &chat) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("chat");
    chat.id = json::value_to<uint64_t>(obj.at("chat_id"));
    chat.name = QString::fromStdString(json::value_to<std::string>(obj.at("chat_name")));
    chat.created_at = QString::fromStdString(json::value_to<std::string>(obj.at("created_at")));
    chat.total_messages = json::value_to<uint64_t>(obj.at("total_messages"));
}

http::request<http::string_body> Handlers::send_message_request(Message &msg) {
    http::request<http::string_body> request;
    std::string image_base64;
    std::string image_binary;
    if (!msg.attachment.isEmpty()) {
        std::ifstream file(msg.attachment.toStdString(), std::ios::binary);
        if (file.good()) {
            file.seekg(0, std::ios::end);
            image_binary.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(image_binary.data(), image_binary.size());
            file.close();

            size_t size = beast::detail::base64::encoded_size(image_binary.size());
            image_base64.resize(size);
            beast::detail::base64::encode((void *)image_base64.data(), image_binary.data(),
                                          image_binary.size());
        } else {
            image_base64 = "";
        }
    } else {
        image_base64 = "";
    }
    json::value request_value = {{"message",
                                  {{"sender_id", msg.senderId},
                                   {"chat_id", msg.chatId},
                                   {"text", msg.text.toStdString()},
                                   {"attachment", image_base64}}}};
    std::string request_body = json::serialize(request_value);
    request.version(11);
    request.method(http::verb::post);
    request.target("/add_message");
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::content_length, std::to_string(request_body.size()));
    request.set(http::field::content_type, "application/json");
    request.body() = request_body;
    return request;
}

void Handlers::send_message_response(http::response<http::string_body> response, Message &msg) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("message");
    msg.id = json::value_to<uint64_t>(obj.at("message_id"));
    msg.created_at = QString::fromStdString(json::value_to<std::string>(obj.at("created_at")));
    msg.text = QString::fromStdString(json::value_to<std::string>(obj.at("text")));
    auto img = json::value_to<std::string>(obj.at("attachment"));
    if (img.empty()) {
        msg.hasAttachment = false;
        msg.attachment = "";
        return;
    }
    QDir dir;
    if (!dir.exists("Images")) {
        if (!dir.mkdir("Images")) {
            msg.hasAttachment = false;
            msg.attachment = "";
            return;
        }
    }
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid1 = generator();
    msg.attachment = dir.currentPath() + "/Images/" + QString::fromStdString(to_string(uuid1)) + ".png";
    msg.hasAttachment = true;
    std::string avatarBinary;
    size_t size = beast::detail::base64::decoded_size(img.size());
    avatarBinary.resize(size);
    beast::detail::base64::decode((void *)avatarBinary.data(), img.data(), img.size());
    std::ofstream file(msg.attachment.toStdString());
    file.write(avatarBinary.data(), size);
    std::cout << response.body() << std::endl;
}

http::request<http::string_body> Handlers::find_chat_by_id_request(Chat &chat) {
    http::request<http::string_body> request;
    request.version(11);
    request.method(http::verb::get);
    request.target("/chat_info?id=" + std::to_string(chat.id));
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.body() = "";
    return request;
}

void Handlers::find_chat_by_id_response(http::response<http::string_body> response, Chat &chat) {
    json::value jv = json::parse(response.body());
    json::object const &obj = jv.as_object();
    jv = obj.at("chat");
    chat.id = json::value_to<uint64_t>(obj.at("chat_id"));
    chat.name = QString::fromStdString(json::value_to<std::string>(obj.at("chat_name")));
    chat.total_messages = json::value_to<uint64_t>(obj.at("total_messages"));
    chat.users_id = json::value_to<std::array<uint64_t, 2>>((obj.at("users_id")));
}
