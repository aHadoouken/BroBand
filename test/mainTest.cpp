#include <gtest/gtest.h>

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QtWidgets/QApplication>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

#include "src/Client.h"
#include "src/Commands.h"
#include "src/Structs.h"

#define SUCCESS 200

TEST(REG_FORM_TEST, EQ) {
    auto cmd = new Commands();

    const QString errors[] = {"Пожалуйста, заполните все поля в регистрационной форме!",
                              "Имя пользователся не должно содержать пробелы!",
                              "Пароли не совпадают! Пожалуйста, попробуйте снова.", "completed"};

    QString username = "";
    QString login = "";
    QString password = "";
    QString confirm_password = password;
    QString str_check = cmd->check_reg_form_task(username, login, password, confirm_password);
    EXPECT_EQ(str_check, errors[0]);

    username = "Андрей ";
    login = "andrew";
    password = "123";
    confirm_password = password;
    str_check = cmd->check_reg_form_task(username, login, password, confirm_password);
    EXPECT_EQ(str_check, errors[1]);

    username = "Андрей";
    login = "andrew";
    password = "123";
    confirm_password = "111";
    str_check = cmd->check_reg_form_task(username, login, password, confirm_password);
    EXPECT_EQ(str_check, errors[2]);

    username = "Андрей";
    login = "andrew";
    password = "123";
    confirm_password = "123";
    str_check = cmd->check_reg_form_task(username, login, password, confirm_password);
    EXPECT_EQ(str_check, errors[3]);

    delete cmd;
}

TEST(LOG_FORM_TEST, EQ) {
    auto cmd = new Commands();

    const QString errors[] = {"Пожалуйста, введите логин и пароль для входа.", "Пожалуйста, введите логин.",
                              "Пожалуйста, введите пароль.", "completed"};

    QString login = "";
    QString password = "";
    QString str_check = cmd->check_log_form_task(login, password);
    EXPECT_EQ(str_check, errors[0]);

    login = "";
    password = "123";
    str_check = str_check = cmd->check_log_form_task(login, password);
    EXPECT_EQ(str_check, errors[1]);

    login = "andrew";
    password = "";
    str_check = str_check = cmd->check_log_form_task(login, password);
    EXPECT_EQ(str_check, errors[2]);

    login = "andrew";
    password = "123";
    str_check = str_check = cmd->check_log_form_task(login, password);
    EXPECT_EQ(str_check, errors[3]);

    delete cmd;
}

void check_reg(User &user, int &er_code) {
    net::io_context ioc;

    auto handlers = new Handlers();
    auto client = new Client(ioc);

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
        er_code = SUCCESS;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{405, "Пользоветль с данным логином уже существует."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {401, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        er_code = err_val;
    }
    delete handlers;
    delete client;
}

TEST(REG_USER_TEST, EQ) {
    User user;
    user.name = "Андрей";
    user.login = "andrew";
    user.password = "123";
    user.profile_avatar = "/home/andrew/Загрузки/avatar.jpg";

    int er_code;

    check_reg(user, er_code);

    ASSERT_EQ(er_code, 405);

    srand(time(0));

    unsigned int j = rand() % 10000 + 1;

    user.name = "Юрий";
    user.login = "login" + QString::number(j);
    user.password = "123";
    user.profile_avatar = "/home/andrew/Загрузки/avatar.jpg";

    check_reg(user, er_code);

    ASSERT_EQ(er_code, SUCCESS);
}

void check_log(User &user, int &er_code) {
    net::io_context ioc;

    auto handlers = new Handlers();
    auto client = new Client(ioc);

    if (client->get_status()) {
        ioc.restart();
    }
    client->run(handlers->login_user_request(user));
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
        handlers->login_user_response(client->get_response(), user);
        client->do_close();
        er_code = SUCCESS;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{401, "Неверный пароль."},
                                          {404, "Неверный логин."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        er_code = err_val;
    }
    delete handlers;
    delete client;
}

TEST(LOG_USER_TEST, EQ) {
    User user;
    user.login = "andsafdrw";
    user.password = "123";

    int er_code;

    check_log(user, er_code);

    ASSERT_EQ(er_code, 404);

    user.login = "andrew";
    user.password = "1243564";

    check_log(user, er_code);

    ASSERT_EQ(er_code, 401);

    user.login = "andrew";
    user.password = "123";

    check_log(user, er_code);

    ASSERT_EQ(er_code, SUCCESS);
}

void check_find_friend(Friend &new_friend, int &er_code) {
    net::io_context ioc;

    auto handlers = new Handlers();
    auto client = new Client(ioc);

    if (client->get_status()) {
        ioc.restart();
    }
    client->run(handlers->find_friend_request(new_friend));
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
        handlers->find_friend_response(client->get_response(), new_friend);
        client->do_close();
        er_code = SUCCESS;
    } catch (int err_val) {
        std::map<int, QString> err_map = {{404, "Пользователь не найден."},
                                          {402, "Ошибка подключения к серверу. Обратитесь к Андрею."},
                                          {400, "Ошибка при отправке запроса. Повторите попытку позже."},
                                          {503, "База данных не отвечает. Повторите попытку позже."}};
        er_code = err_val;
    }

    delete handlers;
    delete client;
}

TEST(FIND_FRIEND_TEST, EQ) {
    Friend new_friend;
    int er_code;

    new_friend.login = "andsaffsdfdrw";
    check_find_friend(new_friend, er_code);
    ASSERT_EQ(er_code, 404);

    new_friend.login = "andrew";
    check_find_friend(new_friend, er_code);
    ASSERT_EQ(er_code, SUCCESS);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
