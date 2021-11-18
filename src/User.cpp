#include "User.h"

User::User() {}

void User::setId(uint64_t id) { this->id = id; }

uint64_t User::getId() { return id; }

void User::setLogin(QString login) { this->login = login; }

QString User::getLogin() { return login; }

void User::setName(QString name) { this->name = name; }

QString User::getName() { return name; }

void User::setStatus(QString status) { this->status = status; }

QString User::getStatus() { return status; }
