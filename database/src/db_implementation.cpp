#include <fstream>
#include <utility>

#include "Exceptions.h"
#include "db_implementation.h"

#define QUERY_BUF_SIZE 256

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

User PostgresDB::AddUser(UserForm userForm) {
    if (userForm.nickname.empty())
        throw InvalidInputs("User nickname is not define");
    char buf[QUERY_BUF_SIZE];
    userForm.nickname = "'" + userForm.nickname + "'";
    userForm.profile_avatar = userForm.profile_avatar.empty() ?
                              "NULL" : "'" + userForm.profile_avatar + "'";
    int res = snprintf(buf, sizeof(buf),
                       "INSERT INTO users(nickname, profile_avatar, created_at)"
                       " VALUES (%s, %s, now())"
                       " returning *;",
                       userForm.nickname.c_str(),
                       userForm.profile_avatar.c_str());
    if (res < 0 || res >= sizeof(buf))
        throw InvalidInputs("Unable to create query string");
    std::string sql(buf);

    pqxx::work W(connect);
    pqxx::result result = W.exec(sql);
    W.commit();

    User user;
    user.id = result[0][0].as<unsigned long>();
    user.nickname = result[0][1].as<std::string>();
    if (!result[0][2].is_null()) {
        user.profile_avatar = result[0][2].as<std::string>();
    }
    user.created_at = result[0][3].as<std::string>();

    return user;
}

User PostgresDB::ExtractUserByID(unsigned long id) {
    throw NotImplemented();
}

User PostgresDB::ExtractUserByNickName(std::string nickname) {
    throw NotImplemented();
}

Chat PostgresDB::AddChat(ChatForm chat) {
    throw NotImplemented();
}

Chat PostgresDB::ExtractChatByID(unsigned long id) {
    throw NotImplemented();
}

std::vector<unsigned long>
PostgresDB::ExtractChatsIDByUserID(unsigned long id) {
    throw NotImplemented();
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
