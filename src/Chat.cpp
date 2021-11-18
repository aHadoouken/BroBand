#include "Chat.h"

Chat::Chat() {
    id = 0;
    creatorId = 0;
    name = " ";
}

void Chat::setId(uint64_t id) { this->id = id; }

uint64_t Chat::getId() { return id; }

void Chat::setCreatorId(uint64_t creatorId) { this->creatorId = creatorId; }

uint64_t Chat::getCreatorId() { return creatorId; }

void Chat::setName(QString name) { this->name = name; }

QString Chat::getName() { return name; }
