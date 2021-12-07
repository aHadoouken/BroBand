#pragma once

#include "structs.h"

class IDataBase {
public:
    virtual User AddUser(UserForm userForm) = 0;

    virtual User GetUserByID(unsigned long id) = 0;

    virtual User GetUserByLogin(const std::string &login) = 0;

    virtual bool Authorization(UserLogin userLogin) = 0;

    virtual Chat AddChat(ChatForm chatForm) = 0;

    virtual Chat GetChatByID(unsigned long id) = 0;

    virtual Message AddMessage(MessageForm msg) = 0;

    virtual std::vector<Message> GetChatMessages(unsigned long chat_id) = 0;

    virtual std::vector<unsigned long>
    ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                          unsigned long last) = 0;

    virtual Message ExtractMessageByID(unsigned long id) = 0;

    virtual std::vector<unsigned long>
    LastMessagesByUserID(unsigned long id) = 0;

};
