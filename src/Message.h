#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include <QVector>
#include <cstdint>

class Message {
   private:
    uint64_t id;
    QString text;
    uint64_t senderId;
    uint64_t chatId;
    bool hasAttachment;
    QVector<QString> attachment;

   public:
    Message();
    void setId(uint64_t);
    uint64_t getId();
    void setText(QString);
    QString getText();
    void setSenderId(uint64_t);
    uint64_t getSenderId();
    void setChatId(uint64_t);
    uint64_t getChatId();
    void setHasAttachment(bool);
    bool getHasAttachment();
    void setAttachment(QVector<QString>);
    QVector<QString> getAttachmetn();
};

#endif  // MESSAGE_H
