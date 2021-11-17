#pragma once

#include "structs.h"

class IDataBase {
private:
    virtual bool OpenConnection() = 0;

    virtual bool CloseConnection() = 0;

public:
    virtual User AddUser(UserForm user) = 0;

    virtual User ExtractUserByID(unsigned long id) = 0;

    virtual User ExtractUserByNickName(std::string nickname) = 0;

    virtual Chat AddChat(ChatForm chat) = 0;

    virtual Chat ExtractChatByID(unsigned long id) = 0;

    virtual std::vector<unsigned long>
    ExtractChatsIDByUserID(unsigned long id) = 0;

    virtual message AddMessage(MessageForm msg) = 0;

    virtual std::vector<unsigned long>
    ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                          unsigned long last) = 0;

    virtual message ExtractMessageByID(unsigned long id) = 0;

    virtual std::vector<unsigned long>
    LastMessagesByUserID(unsigned long id) = 0;

};
