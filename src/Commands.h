#ifndef COMMANDS_H
#define COMMANDS_H

#include <QQueue>
#include <QThread>

class Commands : public QThread {
    Q_OBJECT

   public:
    explicit Commands();

   signals:
    void signIn();
    void signUp();
    void serchUser();
    void addUser();
    void receiveMessage(QString);
    void setUsername(QString);
    void setUserId(QString);
    void reconnect();

   private slots:

   public:
    void run();
    void disconnect();
};

#endif  // COMMANDS_H
