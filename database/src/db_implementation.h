#pragma once

#include <pqxx/pqxx>
#include "db_interface.h"

class PostgresDB : public IDataBase {
public:
    PostgresDB(const std::string &config_path);

    User AddUser(UserForm userForm) override;

    User ExtractUserByID(unsigned long id) override;

    User ExtractUserByNickName(std::string nickname) override;

    Chat AddChat(ChatForm chat) override;

    Chat ExtractChatByID(unsigned long id) override;

    std::vector<unsigned long>
    ExtractChatsIDByUserID(unsigned long id) override;

    message AddMessage(MessageForm msg) override;

    std::vector<unsigned long>
    ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                          unsigned long last) override;

    message ExtractMessageByID(unsigned long id) override;

    std::vector<unsigned long> LastMessagesByUserID(unsigned long id) override;

private:
    pqxx::connection connect;
private:
    std::string ParseConfig(const std::string &config_path);

    PostgresDB() = default;

    PostgresDB(const PostgresDB &);

};
