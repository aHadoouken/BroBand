#ifndef COMMANDS_H
#define COMMANDS_H

#include <QQueue>
#include <QThread>
#include <map>

#include "Client.h"
#include "Structs.h"
#include "handlers.h"

class Commands : public QThread {
    Q_OBJECT

   public:
    explicit Commands();
    ~Commands();

   signals:
    void reg_error_gen(QString);
    void log_error_gen(QString);
    void find_friend_error_gen(QString);
    void create_chat_error_gen(QString);
    void send_message_error_gen(QString);

   public:
    void run();
    void disconnect();
    void stop_connect();
    QString check_reg_form_task(const QString, const QString, const QString, const QString);
    QString check_log_form_task(const QString, const QString);
    bool get_status();
    void connect();
    void reg_user(User &);
    void login_user(User &);
    void find_friend(Friend &);
    void create_chat(Chat &);
    void send_message(Message &);

   private:
    Client *client;
    Handlers *handlers;
    bool status;
};

#endif  // COMMANDS_H
