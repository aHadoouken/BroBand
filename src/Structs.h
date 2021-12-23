#ifndef USERS_H
#define USERS_H

#include <QString>
#include <array>
#include <cstdint>
#include <map>
#include <vector>

typedef struct Chat {
    uint64_t id;
    std::array<uint64_t, 2> users_id;
    QString name;
    uint64_t total_messages;
    uint64_t last_sender_id;
    uint64_t last_msg_id;
    QString created_at;
} Chat;

typedef struct Friend {
    uint64_t id;
    QString login;
    QString name;
    QString created_at;
    QString profile_avatar;
} Friend;

typedef struct Message {
    uint64_t id;
    QString text;
    uint64_t senderId;
    uint64_t chatId;
    bool hasAttachment;
    QString attachment;
    QString created_at;
    QString sender_name;
    QString chat_name;
} Message;

typedef struct User {
    uint64_t id;
    QString login;
    QString name;
    QString password;
    QString status;
    QString created_at;
    QString profile_avatar;
    std::vector<uint64_t> chats_id;
    std::map<QString, Chat> user_chats_by_name;
    std::map<uint64_t, Chat> user_chats_by_id;
    std::map<QString, Friend> user_friend_by_name;
    std::map<QString, Friend> user_friend_by_login;
    std::map<uint64_t, Friend> user_friend_by_id;
    Message msg;
} User;

#endif  // USERS_H
