#pragma once

#include <pqxx/pqxx>
#include "db_interface.h"

class PostgresDB : public IDataBase {
public:
    PostgresDB(const std::string &config_path);

    User AddUser(UserForm userForm) override;

    User ExtractUserByID(unsigned long id) override;

    User ExtractUserByNickName(const std::string &nickname) override;

    Chat AddChat(ChatForm chatForm) override;

    Chat ExtractChatByID(unsigned long id) override;

    Message AddMessage(MessageForm msg) override;

    std::vector<unsigned long>
    ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                          unsigned long last) override;

    Message ExtractMessageByID(unsigned long id) override;

    std::vector<unsigned long> LastMessagesByUserID(unsigned long id) override;

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
};
