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
    user.login = result[0][1].as<std::string>();
    user.name = result[0][2].as<std::string>();
    user.created_at = result[0][5].as<std::string>();
    if (!result[0][4].is_null()) {
        user.profile_avatar = result[0][4].as<std::string>();
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
PostgresDB::FillChatInfoUsers(Chat &chat, const pqxx::result &result) {
    for (pqxx::result::const_iterator r = result.begin();
         r != result.end(); r++) {
        chat.users_id.push_back(r[0].as<unsigned long>());
    }
}

void PostgresDB::FillMessage(Message &message, const pqxx::result &result) {
    message.id = result[0][0].as<unsigned long>();
    message.sender_id = result[0][1].as<unsigned long>();
    message.sender_name = result[0][2].as<std::string>();
    message.chat_id = result[0][3].as<unsigned long>();
    message.chat_name = result[0][4].as<std::string>();
    if (!result[0][5].is_null()) {
        message.text = result[0][5].as<std::string>();
    }
    if (!result[0][6].is_null()) {
        message.attachment = result[0][6].as<std::string>();
    }
    message.created_at = result[0][7].as<std::string>();
}

void PostgresDB::FillMessages(std::vector<Message> &messages,
                              const pqxx::result &result) {
    if (messages.size() != result.size()) {
        throw InvalidInputs();
    }
    for (int i = 0; i < result.size(); i++) {
        messages[i].id = result[i][0].as<unsigned long>();
        messages[i].sender_id = result[i][1].as<unsigned long>();
        messages[i].sender_name = result[i][2].as<std::string>();
        messages[i].chat_id = result[i][3].as<unsigned long>();
        messages[i].chat_name = result[i][4].as<std::string>();
        if (!result[i][5].is_null()) {
            messages[i].text = result[i][5].as<std::string>();
        }
        if (!result[i][6].is_null()) {
            messages[i].attachment = result[i][6].as<std::string>();
        }
        messages[i].created_at = result[i][7].as<std::string>();
    }
}

User PostgresDB::AddUser(UserForm userForm) {
    if (userForm.login.empty() || userForm.name.empty() ||
        userForm.password.empty())
        throw InvalidInputs("User inputs are not define");
    userForm.name = "'" + userForm.name + "'";
    userForm.login = "'" + userForm.login + "'";
    userForm.password = "'" + userForm.password + "'";
    userForm.profile_avatar = userForm.profile_avatar.empty() ?
                              "NULL" : "'" + userForm.profile_avatar + "'";
    std::string sql = (boost::format(
            "INSERT INTO users(login, name, password, profile_avatar, created_at)"
            " VALUES (%s, %s, %s, %s, now())"
            " returning *;")
                       % userForm.login
                       % userForm.name
                       % userForm.password
                       % userForm.profile_avatar).str();

    pqxx::work W(connect);
    pqxx::result result = W.exec(sql);
    W.commit();

    User user;
    FillUserInfoWithoutChats(user, result);
    return user;
}

User PostgresDB::GetUserByID(unsigned long id) {
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

User PostgresDB::GetUserByLogin(const std::string &login) {
    std::string sql = (boost::format(
            "SELECT * FROM users WHERE login='%s';")
                       % login).str();

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

std::string PostgresDB::GetPassword(const std::string &login) {
    std::string sql = (boost::format(
            "SELECT password FROM users WHERE login='%s';")
                       % login).str();

    pqxx::nontransaction N(connect);
    pqxx::result result = N.exec(sql);
    if (result.empty()) {
        throw pqxx::plpgsql_no_data_found("User not found");
    }
    auto password = result[0][0].as<std::string>();
    return password;
}

Chat PostgresDB::AddChat(ChatForm chatForm) {
    if (chatForm.chat_name.empty())
        throw InvalidInputs("Chat name is not define");
    if (chatForm.users_id.empty())
        throw InvalidInputs("Users id for chat are not define");
    // Проверка, что пользователь существует
    for (auto &user_id: chatForm.users_id)
        GetUserByID(user_id);
    chatForm.chat_name = "'" + chatForm.chat_name + "'";
    std::string sql = (boost::format(
            "INSERT INTO chats(chat_name, total_messages, created_at)"
            " VALUES (%s, 0, now())"
            " returning *;")
                       % chatForm.chat_name).str();

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

Chat PostgresDB::GetChatByID(unsigned long id) {
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

Message PostgresDB::AddMessage(MessageForm msg) {
    if (!msg.sender_id || !msg.chat_id ||
        (msg.attachment.empty() && msg.text.empty()))
        throw InvalidInputs("Invalid inputs for MessageForm");
    // Проверка, что чат существвует
    GetChatByID(msg.chat_id);
    // Проверка, что отправитель существует и он есть в указанном чате
    auto user = GetUserByID(msg.sender_id);
    if (std::find(user.chats_id.begin(), user.chats_id.end(), msg.chat_id) ==
        user.chats_id.end()) {
        throw InvalidInputs("The user is not a member of the chat");
    }
    msg.text = msg.text.empty() ?
               "NULL" : "'" + msg.text + "'";
    msg.attachment = msg.attachment.empty() ?
                     "NULL" : "'" + msg.attachment + "'";
    std::string sql = (boost::format(
            "UPDATE chats "
            "SET total_messages = total_messages + 1 "
            "WHERE id = %lu;"
            "INSERT INTO messages(sender_id, chat_id, text, attachment, created_at)"
            " VALUES (%lu, %lu, %s, %s, now())"
            " returning id;")
                       % msg.chat_id
                       % msg.sender_id
                       % msg.chat_id
                       % msg.text
                       % msg.attachment).str();

    pqxx::work W(connect);
    pqxx::result result = W.exec(sql);
    W.commit();

    auto id = result[0][0].as<unsigned long>();
    sql = (boost::format(
            "SELECT m.id, m.sender_id, users.name, m.chat_id, chats.chat_name, "
            "m.text, m.attachment, m.created_at "
            "FROM messages as m "
            "left join chats on (m.chat_id = chats.id) "
            "left join users on (m.sender_id = users.user_id) "
            "where m.id = %lu;")
                       % id).str();
    pqxx::nontransaction N(connect);
    result = N.exec(sql);

    Message message;
    FillMessage(message, result);
    return message;
}

std::vector<Message> PostgresDB::GetChatMessages(unsigned long chat_id) {
    std::string sql = (boost::format(
            "SELECT m.id, m.sender_id, users.name, m.chat_id, chats.chat_name, "
            "m.text, m.attachment, m.created_at "
            "FROM messages as m "
            "left join chats on (m.chat_id = chats.id) "
            "left join users on (m.sender_id = users.user_id) "
            "where chats.id = %lu;")
                       % chat_id).str();

    pqxx::nontransaction N(connect);
    pqxx::result result = N.exec(sql);
    std::vector<Message> messages;
    messages.resize(result.size());

    FillMessages(messages, result);

    return messages;
}

std::vector<Message> PostgresDB::GetChatMessagesAfterID(unsigned long chat_id,
                                            unsigned long msg_id){
    std::string sql = (boost::format(
            "SELECT m.id, m.sender_id, users.name, m.chat_id, chats.chat_name, "
            "m.text, m.attachment, m.created_at "
            "FROM messages as m "
            "left join chats on (m.chat_id = chats.id) "
            "left join users on (m.sender_id = users.user_id) "
            "where chats.id = %lu and m.id > %lu;")
                       % chat_id
                       % msg_id).str();

    pqxx::nontransaction N(connect);
    pqxx::result result = N.exec(sql);
    std::vector<Message> messages;
    messages.resize(result.size());

    FillMessages(messages, result);

    return messages;
}
