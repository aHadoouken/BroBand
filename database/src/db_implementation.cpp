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

User PostgresDB::ExtractUserByID(int id) {
    throw NotImplemented();
}

User PostgresDB::ExtractUserByNickName(std::string NickName) {
    throw NotImplemented();
}

Chat PostgresDB::AddChat(ChatForm chat) {
    throw NotImplemented();
}

Chat PostgresDB::ExtractChatByID(int id) {
    throw NotImplemented();
}

std::vector<int> PostgresDB::ExtractChatsIDByUserID(int id) {
    throw NotImplemented();
}

Message PostgresDB::AddMessage(MessageForm msg) {
    throw NotImplemented();
}

std::vector<int> PostgresDB::ExtractChatMessagesID(int chatID, int first, int last) {
    throw NotImplemented();
}

Message PostgresDB::ExtractMessageByID(int id){
    throw NotImplemented();
}

std::vector<int> PostgresDB::LastMessagesByUserID(int id) {
    throw NotImplemented();
}
