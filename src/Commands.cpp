#include "Commands.h"

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

QString Commands::check_reg_form_task(const QString username, const QString login,
                                           const QString password, const QString confirm_password)
{
    const QString signup_error[] = {"Пожалуйста, заполните все поля в регистрационной форме!",
                                    "Пользователь с таким логином уже существует!",
                                    "Имя пользователся не должно содержать пробелы!",
                                    "Пожалуйста, подтвердите пароль.",
                                    "Пароли не совпадают! Пожалуйста, попробуйте снова.",
                                    "Данный пользователь уже зарегистрирован!",
                                    "Не удалось установить соединение с сервером! Пожалуйста, попробуйте позднее!",
                                    "completed"};

    if ((username.isEmpty()) || (login.isEmpty()) || (password.isEmpty()) || confirm_password.isEmpty()) {
        return signup_error[0];
    }
    else if (username.contains(' ')) {
        return signup_error[2];
    }
    else if (confirm_password.isEmpty()) {
        return signup_error[3];
    }
    else if (confirm_password != password) {
        return signup_error[4];
    }
    else {
        return signup_error[7];
    }
}

QString Commands::check_log_form_task(const QString login, const QString password)
{
    const QString signin_error[] = {"Пожалуйста, введите логин и пароль для входа.",
                                    "Пожалуйста, введите логин.",
                                    "Пожалуйста, введите пароль.",
                                    "completed"};

    if (login.isEmpty() || password.isEmpty()) {
       return signin_error[0];
    }
    else if (login.isEmpty()) {
        return signin_error[1];
    }
    else if (password.isEmpty()) {
        return signin_error[2];
    }
    else {
        return signin_error[3];
    }
}

void Commands::reg_user(User &user)
{
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
    }
    catch (int err_val)
    {
        std::map<int, QString> err_map = {{405, "Пользоветль с данным логином уже существует."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {401, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit reg_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::login_user(User  &user)
{

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
        status = true;
        //ioc.restart();
        //client->do_write();
        //ioc.run();
    }
    catch (int err_val)
    {
        std::map<int, QString> err_map = {{401, "Неверный пароль."},
                                          {404, "Неверный логин."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit log_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::find_friend(Friend &new_friend)
{
    ioc.restart();
    client->do_write(handlers->find_friend_request(new_friend));
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
    }
    catch (int err_val)
    {
        std::map<int, QString> err_map = {{404, "Пользователь не найден."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit find_friend_error_gen(err_map.find(err_val)->second);
    }

}

void Commands::create_chat(Chat &chat)
{
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
    }
    catch (int err_val)
    {
        std::map<int, QString> err_map = {{404, "Собеседника не существует."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit create_chat_error_gen(err_map.find(err_val)->second);
    }
}

void Commands::send_message(Message &message)
{
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
        status = true;
    }
    catch (int err_val)
    {
        std::map<int, QString> err_map = {{404, "Пользователя не существует."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        status = false;
        emit send_message_error_gen(err_map.find(err_val)->second);
    }
}

bool Commands::get_status()
{
    return status;
}

void Commands::stop_connect()
{
    client->do_close();
}


