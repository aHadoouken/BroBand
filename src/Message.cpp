#include "Message.h"

Message::Message() {
    id = 0;
    text = " ";
    senderId = 0;
    chatId = 0;
    hasAttachment = false;
}

void Message::setId(uint64_t id) { this->id = id; }

uint64_t Message::getId() { return id; }

void Message::setText(QString text) { this->text = text; }

QString Message::getText() { return text; }

void Message::setSenderId(uint64_t senderId) { this->senderId = senderId; }

uint64_t Message::getSenderId() { return senderId; }

void Message::setChatId(uint64_t chatId) { this->chatId = chatId; }

uint64_t Message::getChatId() { return chatId; }

void Message::setHasAttachment(bool hasAttachment) { this->hasAttachment = hasAttachment; }

bool Message::getHasAttachment() { return hasAttachment; }

void Message::setAttachment(QVector<QString> attachment) { this->attachment = attachment; }

QVector<QString> Message::getAttachmetn() { return attachment; }
