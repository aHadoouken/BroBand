#pragma once

#include <exception>
#include "db_interface.h"

class PostgresDB : public IDataBase {
private:
    bool OpenConnection() override;

    bool CloseConnection() override;

public:
    User AddUser(UserForm user) override;

    User ExtractUserByID(int id) override;

    User ExtractUserByNickName(std::string NickName) override;

    Chat AddChat(ChatForm chat) override;

    Chat ExtractChatByID(int id) override;

    std::vector<int> ExtractChatsIDByUserID(int id) override;

    Message AddMessage(MessageForm msg) override;

    std::vector<int> ExtractChatMessagesID(int chatID, int first, int last) override;

    Message ExtractMessageByID(int id) override;

    std::vector<int> LastMessagesByUserID(int id) override;

};

class NotImplemented : public std::exception {
private:
    std::string m_error;

public:
    NotImplemented(std::string error = "Function not yet implemented")
            : m_error(error) {
    }

    const char *
    what() const noexcept override { return m_error.c_str(); } // C++11 и выше
};

class InvalidInputs : public std::exception {
private:
    std::string m_error;

public:
    InvalidInputs(std::string error = "Invalid inputs")
            : m_error(error) {
    }

    const char *
    what() const noexcept override { return m_error.c_str(); } // C++11 и выше
};

