#ifndef USERS_H
#define USERS_H
#include <QString>
#include <cstdint>

class User {
   private:
    uint64_t id;
    QString login;
    QString name;
    QString status;

   public:
    User();
    void setId(uint64_t);
    uint64_t getId();
    void setLogin(QString);
    QString getLogin();
    void setName(QString);
    QString getName();
    void setStatus(QString);
    QString getStatus();
};

#endif  // USERS_H
