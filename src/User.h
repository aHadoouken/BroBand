#ifndef USERS_H
#define USERS_H

#include <QString>
#include <QVector>
#include <cstdint>

typedef struct User {
    uint64_t id;
    QString login;
    QString name;
    QString password;
    QString status;
} User;

typedef struct Chat {
    uint64_t id;
    uint64_t creatorId;
    QString name;
} Chat;

typedef struct Message {
    uint64_t id;
    QString text;
    uint64_t senderId;
    uint64_t chatId;
    bool hasAttachment;
    QVector<QString> attachment;
} Message;

#endif  // USERS_H
