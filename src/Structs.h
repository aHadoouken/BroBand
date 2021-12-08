#ifndef USERS_H
#define USERS_H

#include <QString>
#include <cstdint>

typedef struct Chat {
    uint64_t id;
    std::array<uint64_t, 2> users_id;
    QString name;
    uint64_t total_messages;
    QString created_at;
} Chat;

typedef struct User {
    uint64_t id;
    QString login;
    QString name;
    QString password;
    QString status;
    QString created_at;
    QString profile_avatar;
    std::vector<uint64_t> chats_id;
    std::vector<Chat> user_chats;
} User;

typedef struct Message {
    uint64_t id;
    QString text;
    uint64_t senderId;
    uint64_t chatId;
    bool hasAttachment;
    QString attachment;
    QString created_at;
} Message;

typedef struct Friend {
    uint64_t id;
    QString login;
    QString name;
    QString created_at;
    QString profile_avatar;
} Friend;

#endif  // USERS_H
