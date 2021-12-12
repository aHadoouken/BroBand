#pragma once

#include <string>
#include <vector>

typedef struct User {
    unsigned long id;
    std::string login;
    std::string name;
    std::string profile_avatar;
    std::string created_at;
    std::vector<unsigned long> chats_id;
} User;

typedef struct UserForm {
    std::string login;
    std::string name;
    std::string password;
    std::string profile_avatar;
} UserForm;

typedef struct Chat {
    unsigned long id;
    std::string chat_name;
    std::string created_at;
    unsigned long total_messages;
    std::vector<unsigned long> users_id;
} Chat;

typedef struct ChatForm {
    std::string chat_name;
    std::vector<unsigned long> users_id;
} ChatForm;

typedef struct Message {
    unsigned long id;
    unsigned long sender_id;
    std::string sender_name;
    unsigned long chat_id;
    std::string chat_name;
    std::string text;
    std::string attachment;
    std::string created_at;
} Message;

typedef struct MessageForm {
    unsigned long sender_id;
    unsigned long chat_id;
    std::string text;
    std::string attachment;
} MessageForm;
