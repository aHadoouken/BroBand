#pragma once

#include <pqxx/pqxx>
#include "db_interface.h"

class PostgresDB : public IDataBase {
public:
    PostgresDB(const std::string &config_path);

    User AddUser(UserForm userForm) override;

    User GetUserByID(unsigned long id) override;

    User GetUserByLogin(const std::string &login) override;

    std::string GetPassword(const std::string &login) override;

    Chat AddChat(ChatForm chatForm) override;

    Chat GetChatByID(unsigned long id) override;

    Message AddMessage(MessageForm msg) override;

    std::vector<Message> GetChatMessages(unsigned long chat_id) override;

    std::vector<Message> GetChatMessagesAfterID(unsigned long chat_id,
                                                unsigned long msg_id) override;

private:
    pqxx::connection connect;
private:
    std::string ParseConfig(const std::string &config_path);

    PostgresDB() = default;

    PostgresDB(const PostgresDB &);

    void FillUserInfoWithoutChats(User &user, const pqxx::result &result);

    void FillUserInfoChats(User &user, const pqxx::result &result);

    void FillChatInfoWithoutUsers(Chat &chat, const pqxx::result &result);

    void FillChatInfoUsers(Chat &chat, const pqxx::result &result);

    void FillMessage(Message &message, const pqxx::result &result);

    void
    FillMessages(std::vector<Message> &messages, const pqxx::result &result);
};
