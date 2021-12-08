#include "User.h"

User::User() {
    id = 0;
    login = "";
    name = "";
    status = "";
    password = "";
}

void User::setId(uint64_t id) { this->id = id; }

uint64_t User::getId() { return id; }

void User::setLogin(QString login) { this->login = login; }

QString User::getLogin() { return login; }

void User::setName(QString name) { this->name = name; }

QString User::getName() { return name; }

void User::setPassword(QString password) { this->password = password; }

QString User::getPassword() { return password; }

void User::setStatus(QString status) { this->status = status; }

QString User::getStatus() { return status; }
