#pragma once

#include "db_interface.h"

class PostgresDB : public IDataBase {
private:
    bool OpenConnection() override;

    bool CloseConnection() override;

public:
    User AddUser(UserForm user) override;

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

};
