#include <fstream>
#include <utility>
#include "boost/format.hpp"

#include "Exceptions.h"
#include "db_implementation.h"

PostgresDB::PostgresDB(const std::string &config_path) {
    std::string connectParams = ParseConfig(config_path);
    connect = pqxx::connection(connectParams);
    if (!connect.is_open()) {
        throw pqxx::broken_connection();
    }
}

std::string PostgresDB::ParseConfig(const std::string &config_path) {
    std::ifstream file(config_path);
    std::map<std::string, std::string> map;
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            line.erase(std::remove_if(line.begin(), line.end(), isspace),
                       line.end());
            if (line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find('=');
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            map.insert(make_pair(name, value));
        }
    } else {
        throw InvalidInputs("Unable to open config file");
    }
    std::string connectParams;
    for (auto &it: map) {
        connectParams += it.first + " = " + it.second + " ";
    }
    return connectParams;
}

void
PostgresDB::FillUserInfoWithoutChats(User &user, const pqxx::result &result) {
    user.id = result[0][0].as<unsigned long>();
    user.nickname = result[0][1].as<std::string>();
    user.created_at = result[0][3].as<std::string>();
    if (!result[0][2].is_null()) {
        user.profile_avatar = result[0][2].as<std::string>();
    }
}

void PostgresDB::FillUserInfoChats(User &user, const pqxx::result &result) {
    for (pqxx::result::const_iterator r = result.begin();
         r != result.end(); r++) {
        user.chats_id.push_back(r[0].as<unsigned long>());
    }
}

void
PostgresDB::FillChatInfoWithoutUsers(Chat &chat, const pqxx::result &result) {
    chat.id = result[0][0].as<unsigned long>();
    chat.chat_name = result[0][1].as<std::string>();
    chat.total_messages = result[0][2].as<unsigned long>();
    chat.created_at = result[0][3].as<std::string>();
}

void
PostgresDB::FillChatInfoUsers(Chat &chat, const pqxx::result &result){
    for (pqxx::result::const_iterator r = result.begin();
         r != result.end(); r++) {
        chat.users_id.push_back(r[0].as<unsigned long>());
    }
}

User PostgresDB::AddUser(UserForm userForm) {
    if (userForm.nickname.empty())
        throw InvalidInputs("User nickname is not define");
    userForm.nickname = "'" + userForm.nickname + "'";
    userForm.profile_avatar = userForm.profile_avatar.empty() ?
                              "NULL" : "'" + userForm.profile_avatar + "'";
    std::string sql = (boost::format(
            "INSERT INTO users(nickname, profile_avatar, created_at)"
            " VALUES (%s, %s, now())"
            " returning *;")
                       % userForm.nickname.data()
                       % userForm.profile_avatar.data()).str();

    pqxx::work W(connect);
    pqxx::result result = W.exec(sql);
    W.commit();

    User user;
    FillUserInfoWithoutChats(user, result);
    return user;
}

User PostgresDB::ExtractUserByID(unsigned long id) {
    std::string sql = (boost::format(
            "SELECT * FROM users WHERE user_id=%lu;")
                       % id).str();

    pqxx::nontransaction N(connect);
    pqxx::result result = N.exec(sql);
    if (result.empty()) {
        throw pqxx::plpgsql_no_data_found("User not found");
    }
    User user;
    FillUserInfoWithoutChats(user, result);

    sql = (boost::format(
            "select chat_id from user_chats where user_id=%lu;")
           % id).str();

    result = N.exec(sql);

    FillUserInfoChats(user, result);

    return user;
}

User PostgresDB::ExtractUserByNickName(const std::string &nickname) {
    std::string sql = (boost::format(
            "SELECT * FROM users WHERE nickname='%s';")
                       % nickname.data()).str();

    pqxx::nontransaction N(connect);
    pqxx::result result = N.exec(sql);
    if (result.empty()) {
        throw pqxx::plpgsql_no_data_found("User not found");
    }
    User user;
    FillUserInfoWithoutChats(user, result);

    sql = (boost::format(
            "select chat_id from user_chats where user_id=%lu;")
           % user.id).str();
    result = N.exec(sql);

    FillUserInfoChats(user, result);

    return user;
}

Chat PostgresDB::AddChat(ChatForm chatForm) {
    if (chatForm.chat_name.empty())
        throw InvalidInputs("Chat name is not define");
    if (chatForm.users_id.empty())
        throw InvalidInputs("Users id for chat are not define");
    // Проверка, что пользователь существует
    for (auto &user_id: chatForm.users_id)
        ExtractUserByID(user_id);
    chatForm.chat_name = "'" + chatForm.chat_name + "'";
    std::string sql = (boost::format(
            "INSERT INTO chats(chat_name, total_messages, created_at)"
            " VALUES (%s, 0, now())"
            " returning *;")
                       % chatForm.chat_name.data()).str();

    pqxx::work W(connect);
    pqxx::result result = W.exec(sql);
    W.commit();

    Chat chat;
    FillChatInfoWithoutUsers(chat, result);

    for (auto &user_id: chatForm.users_id) {
        //TODO: сделать один запрос
        pqxx::work W(connect);
        sql = (boost::format(
                "INSERT INTO user_chats(user_id, chat_id)"
                " VALUES (%lu, %lu)")
               % user_id
               % chat.id).str();
        W.exec(sql);
        W.commit();
    }
    // Возможно стоит делать sql запрос и оттуда вытаскивать users_id,
    // чтобы ничего не потерялось
    chat.users_id = chatForm.users_id;

    return chat;
}

Chat PostgresDB::ExtractChatByID(unsigned long id) {
    std::string sql = (boost::format(
            "SELECT * FROM chats WHERE id=%lu;")
                       % id).str();

    pqxx::nontransaction N(connect);
    pqxx::result result = N.exec(sql);
    if (result.empty()) {
        throw pqxx::plpgsql_no_data_found("Chat not found");
    }
    Chat chat;
    FillChatInfoWithoutUsers(chat, result);

    sql = (boost::format(
            "select user_id from user_chats where chat_id=%lu;")
           % id).str();

    result = N.exec(sql);
    FillChatInfoUsers(chat, result);
    return chat;
}

message PostgresDB::AddMessage(MessageForm msg) {
    throw NotImplemented();
}

std::vector<unsigned long>
PostgresDB::ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                                  unsigned long last) {
    throw NotImplemented();
}

message PostgresDB::ExtractMessageByID(unsigned long id) {
    throw NotImplemented();
}

std::vector<unsigned long> PostgresDB::LastMessagesByUserID(unsigned long id) {
    throw NotImplemented();
}
