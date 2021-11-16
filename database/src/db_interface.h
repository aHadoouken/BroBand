#pragma once

#include "structs.h"

class IDataBase {
private:
    virtual bool OpenConnection() = 0;

    virtual bool CloseConnection() = 0;

public:
    virtual User AddUser(UserForm user) = 0;

    virtual User ExtractUserByID(int id) = 0;

    virtual User ExtractUserByNickName(std::string NickName) = 0;

    virtual Chat AddChat(ChatForm chat) = 0;

    virtual Chat ExtractChatByID(int id) = 0;

    virtual std::vector<int> ExtractChatsIDByUserID(int id) = 0;

    virtual Message AddMessage(MessageForm msg) = 0;

    virtual std::vector<int> ExtractChatMessagesID(int chatID, int first, int last) = 0;

    virtual Message ExtractMessageByID(int id) = 0;

    virtual std::vector<int> LastMessagesByUserID(int id) = 0;

};
