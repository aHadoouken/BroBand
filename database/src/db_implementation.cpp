#include "Exceptions.h"
#include "db_implementation.h"

bool PostgresDB::OpenConnection() {
    return true;
}

bool PostgresDB::CloseConnection() {
    return true;
}

User PostgresDB::AddUser(UserForm user) {
    throw NotImplemented();
}

User PostgresDB::ExtractUserByID(unsigned long id) {
    throw NotImplemented();
}

User PostgresDB::ExtractUserByNickName(std::string nickname) {
    throw NotImplemented();
}

Chat PostgresDB::AddChat(ChatForm chat) {
    throw NotImplemented();
}

Chat PostgresDB::ExtractChatByID(unsigned long id) {
    throw NotImplemented();
}

std::vector<unsigned long>
PostgresDB::ExtractChatsIDByUserID(unsigned long id) {
    throw NotImplemented();
}

message PostgresDB::AddMessage(MessageForm msg) {
    throw NotImplemented();
}

std::vector<unsigned long>
PostgresDB::ExtractChatMessagesID(unsigned long chat_id, unsigned long first,
                                  unsigned long last) {
    throw NotImplemented();
}

message PostgresDB::ExtractMessageByID(unsigned long id) {
    throw NotImplemented();
}

std::vector<unsigned long> PostgresDB::LastMessagesByUserID(unsigned long id) {
    throw NotImplemented();
}
