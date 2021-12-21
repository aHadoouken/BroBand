#include "Commands.h"
#include <unistd.h>
#include <QWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QLabel>

#define SUCCESS 200

net::io_context ioc;

Commands::Commands() {
    client = new Client(ioc);
    handlers = new Handlers();
    status = false;
}

Commands::~Commands() {
    delete client;
    delete handlers;
}


void Commands::run() { exec(); }

void Commands::disconnect() { exit(0); }

QString Commands::check_reg_form_task(const QString username, const QString login, const QString password,
                                      const QString confirm_password) {
    const QString signup_error[] = {"Пожалуйста, заполните все поля в регистрационной форме!",
                                    "Имя пользователся не должно содержать пробелы!",
                                    "Пароли не совпадают! Пожалуйста, попробуйте снова.", "completed"};

    if ((username.isEmpty()) || (login.isEmpty()) || (password.isEmpty()) || confirm_password.isEmpty()) {
        return signup_error[0];
    } else if (username.contains(' ')) {
        return signup_error[1];
    } else if (confirm_password != password) {
        return signup_error[2];
    } else {
        return signup_error[3];
    }
}

QString Commands::check_log_form_task(const QString login, const QString password) {
    const QString signin_error[] = {"Пожалуйста, введите логин и пароль для входа.",
                                    "Пожалуйста, введите логин.", "Пожалуйста, введите пароль.", "completed"};

    if (login.isEmpty() && password.isEmpty()) {
        return signin_error[0];
    } else if (login.isEmpty()) {
        return signin_error[1];
    } else if (password.isEmpty()) {
        return signin_error[2];
    } else {
        return signin_error[3];
    }
}

void Commands::reg_user(User &user) {
    if (client->get_status()) {
        ioc.restart();
    }
    client->run(handlers->add_user_request(user));
    ioc.run();
    int response_result = client->get_response().result_int();
    try {
        if (!client->get_status()) {
            int err = 401;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        handlers->add_user_response(client->get_response(), user);
        client->do_close();
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {405, "Пользоветль с данным логином уже существует."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {401, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {502, "Сервер временно не доступен. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit reg_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::login_user(User &user) {
    if (client->get_status()) {
        ioc.restart();
    }
    client->run(handlers->login_user_request(user));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        handlers->login_user_response(client->get_response(), user);
        user.status = "Online";
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {401, "Неверный пароль."},
                                          {404, "Неверный логин."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {502, "Сервер временно не доступен. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit log_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::find_friend(Friend &new_friend) {
    QString mode;
    if (!new_friend.login.isEmpty()) {
        mode = "by_login";
    }
    else {
        mode = "by_id";
    }
    ioc.restart();
    client->do_write(handlers->find_friend_request(new_friend, mode));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        handlers->find_friend_response(client->get_response(), new_friend);
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Пользователь не найден."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit find_friend_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::create_chat(Chat &chat) {
    ioc.restart();
    client->do_write(handlers->create_chat_request(chat));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        handlers->create_chat_response(client->get_response(), chat);
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Собеседника не существует."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit create_chat_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::send_message(Message &message) {
    ioc.restart();
    client->do_write(handlers->send_message_request(message));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        handlers->send_message_response(client->get_response(), message);
        QString mode = "user";
        emit start_form_new_msg(mode, message);
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Пользователя не существует."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit send_message_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::get_user_chat(Chat &chat, User &user)
{
    ioc.restart();
    client->do_write(handlers->find_chat_by_id_request(chat));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        handlers->find_chat_by_id_response(client->get_response(), chat);
        user.user_chats_by_id.emplace(chat.id, chat);
        user.user_chats_by_name.emplace(chat.name, chat);
        for (size_t i = 0; i < chat.users_id.size(); ++i) {
            if (chat.users_id.at(i) != user.id) {
                Friend old_friend;
                old_friend.id = chat.users_id.at(i);
                find_friend(old_friend);
                user.user_friend_by_name.emplace(old_friend.name, old_friend);
                user.user_friend_by_login.emplace(old_friend.login, old_friend);
                user.user_friend_by_id.emplace(old_friend.id, old_friend);
            }
        }
        emit start_create_chat(chat);
        if (!get_all_chat_msg(chat, user)) {
            bool get_msg_err = true;
            throw get_msg_err;
        }

        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Чата не существует."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit create_chat_error_gen(err_map.find(err_val)->second);
        return;
    }
    catch (bool get_msg_err)
    {
        QString err_get_msg = "Не удалось воосстановить историю сообщений.";
        emit create_chat_error_gen(err_get_msg);
        status = false;
        return;
    }
}

bool Commands::get_all_chat_msg(Chat &chat, User &user)
{
    if (chat.total_messages == 0)
    {
        user.user_chats_by_id.find(chat.id)->second.last_msg_id = 0;
        user.user_chats_by_id.find(chat.id)->second.last_sender_id = 0;
        user.user_chats_by_name.find(chat.name)->second.last_msg_id = 0;
        user.user_chats_by_name.find(chat.name)->second.last_sender_id = 0;
        return true;
    }
    ioc.restart();
    client->do_write(handlers->get_all_chat_msg_request(chat));
    ioc.run();
    int response_result = client->get_response().result_int();
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        std::vector<Message> new_messages;
        new_messages = handlers->get_all_chat_msg_response(client->get_response());
        for (size_t i = 0; i < new_messages.size(); ++i)
        {
            user.user_chats_by_id.find(chat.id)->second.last_msg_id = new_messages.at(i).id;
            user.user_chats_by_id.find(chat.id)->second.last_sender_id = new_messages.at(i).senderId;
            user.user_chats_by_name.find(chat.name)->second.last_msg_id = new_messages.at(i).id;
            user.user_chats_by_name.find(chat.name)->second.last_sender_id = new_messages.at(i).senderId;
            QString mode;
            if (new_messages.at(i).senderId == user.id)
                mode = "user";
            else
                mode = "friend";
            emit start_form_new_msg(mode, new_messages.at(i));
        }
        status = true;
        return true;
    }
    catch (int err_val)
    {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Чата не существует."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit create_chat_error_gen(err_map.find(err_val)->second);
        return false;
    }
}

void Commands::update_chats(struct User &user)
{
    ioc.restart();
    client->do_write(handlers->update_user_info_request(user));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        std::vector<uint64_t> update_chats_id;
        update_chats_id = handlers->update_user_info_response(client->get_response());
        if (user.chats_id.size() != update_chats_id.size())
        {
            for (size_t i = user.chats_id.size(); i < update_chats_id.size(); ++i)
            {
                Chat new_chat;
                new_chat.id = update_chats_id.at(i);
                get_user_chat(new_chat, user);
            }
        }
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Пользователь не найден."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit create_chat_error_gen(err_map.find(err_val)->second);
    }
}


void Commands::update_current_chat(Chat &chat, User &user)
{
    ioc.restart();
    client->do_write(handlers->update_current_chat_request(chat, user));
    ioc.run();
    int response_result = client->get_response().result_int();
    std::cout << response_result << std::endl;
    try {
        if (!client->get_status()) {
            int err = 402;
            throw err;
        }
        if (response_result != SUCCESS) {
            throw response_result;
        }
        std::vector<Message> new_messages;
        new_messages = handlers->update_current_chat_response(client->get_response());
        for (size_t i = 0; i < new_messages.size(); ++i)
        {
            if (new_messages.at(i).senderId == user.id)
                continue;
            user.user_chats_by_id.find(chat.id)->second.last_msg_id = new_messages.at(i).id;
            user.user_chats_by_id.find(chat.id)->second.last_sender_id = new_messages.at(i).senderId;

            user.user_chats_by_name.find(chat.name)->second.last_msg_id = new_messages.at(i).id;
            user.user_chats_by_name.find(chat.name)->second.last_sender_id = new_messages.at(i).senderId;
            QString mode;
            mode = "friend";
            emit start_form_new_msg(mode, new_messages.at(i));
        }
        status = true;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{303, "Сервер не отвечает."},
                                          {404, "Пользователь не найден."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit create_chat_error_gen(err_map.find(err_val)->second);
    }
}

bool Commands::get_status() { return status; }

void Commands::stop_connect() { client->do_close(); }

void Commands::del_current_user(User &user)
{
    user.user_chats_by_id.clear();
    user.user_chats_by_name.clear();
    user.user_friend_by_id.clear();
    user.user_friend_by_name.clear();
    user.chats_id.clear();
}
