#ifndef COMMANDS_H
#define COMMANDS_H

#include <QQueue>
#include <QThread>
#include <boost/thread/thread.hpp>
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
    void start_form_new_msg(QString, Message);
    void start_create_chat(Chat);

   public slots:
    void run();

   public:
    void disconnect();
    void stop_connect();
    QString check_reg_form_task(const QString, const QString, const QString, const QString);
    QString check_log_form_task(const QString, const QString);
    bool get_status();
    void reg_user(User &);
    void login_user(User &);
    void find_friend(Friend &);
    void create_chat(Chat &);
    void send_message(Message &);
    void get_user_chat(Chat &, User &);
    bool get_all_chat_msg(Chat &, User &);
    void update_chats(struct User &);
    void update_current_chat(Chat &, User &);

   private:
    Client *client;
    Handlers *handlers;
    bool status;
};

#endif  // COMMANDS_H
