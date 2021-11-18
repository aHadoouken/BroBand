#ifndef CHAT_H
#define CHAT_H
#include <QString>
#include <cstdint>

class Chat {
   private:
    uint64_t id;
    uint64_t creatorId;
    QString name;

   public:
    Chat();
    void setId(uint64_t);
    uint64_t getId();
    void setCreatorId(uint64_t);
    uint64_t getCreatorId();
    void setName(QString);
    QString getName();
};

#endif  // CHAT_H
