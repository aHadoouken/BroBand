#pragma once

#include "structs.h"

class IDataBase {
public:
    virtual User AddUser(UserForm userForm) = 0;

    virtual User ExtractUserByID(unsigned long id) = 0;

    virtual User ExtractUserByNickName(const std::string &nickname) = 0;

    virtual Chat AddChat(ChatForm chatForm) = 0;

    virtual Chat ExtractChatByID(unsigned long id) = 0;

    virtual Message AddMessage(MessageForm msg) = 0;

    virtual std::vector<unsigned long>
    ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                          unsigned long last) = 0;

    virtual Message ExtractMessageByID(unsigned long id) = 0;

    virtual std::vector<unsigned long>
    LastMessagesByUserID(unsigned long id) = 0;

};
