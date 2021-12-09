#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
    this->menuBar()->hide();
    this->statusBar()->hide();
    ui.stackedWidget->setCurrentWidget(ui.SignIn);
    QPixmap avatar(":/Resources/profile.png");
    ui.signup_avatar->setPixmap(avatar);
    ui.messendger_avatar->setPixmap(avatar);
    ui.chat_windows->setCurrentWidget(ui.empty);


    cmd = new Commands();
    cmd->start();

    connect(cmd, SIGNAL(reg_error_gen(QString)), this, SLOT(print_reg_error(QString)));
    connect(cmd, SIGNAL(log_error_gen(QString)), this, SLOT(print_log_error(QString)));
    connect(cmd, SIGNAL(create_chat_error_gen(QString)), this, SLOT(print_create_chat_error(QString)));
    connect(cmd, SIGNAL(send_message_error_gen(QString)), this, SLOT(print_send_message_error(QString)));
    connect(cmd, SIGNAL(find_friend_error_gen(QString)), this, SLOT(print_find_friend_error(QString)));
}

MainWindow::~MainWindow() {
    cmd->disconnect();
    delete cmd;
}




void MainWindow::on_signin_signup_button_clicked()
{
    ui.stackedWidget->setCurrentWidget(ui.SignUp);
    ui.signin_error_label->setStyleSheet("color: rgb(239, 41, 41);");
}

void MainWindow::on_signin_signin_button_clicked()
{
    ui.signin_error_label->setStyleSheet("color: rgb(239, 41, 41);");
    QString login = ui.signin_login_edit->text();
    QString password = ui.signin_password_edit->text();
    QString str_check = cmd->check_log_form_task(login, password);
    if (str_check != "completed") {
        ui.signin_error_label->setText(str_check);
        return;
    }
    ui.signin_error_label->setText("");
    user.login = login;
    user.password = password;
    cmd->login_user(user);
    if (!cmd->get_status()) {
        return;
    }
    user.status = "Online";
    ui.username_label->setText(user.name);
    ui.status_connection_label->setText(user.status);

    ui.stackedWidget->setCurrentWidget(ui.Messanger);
    this->menuBar()->show();
    this->statusBar()->show();
    ui.signin_error_label->setText("");
    ui.signin_login_edit->setText("");
    ui.signin_password_edit->setText("");
}


void MainWindow::on_signup_haveAcc_button_clicked()
{
    ui.stackedWidget->setCurrentWidget(ui.SignIn);
    ui.signup_error_label->setText("");
    ui.username_edit->setText("");
    ui.login_edit->setText("");
    ui.password_edit->setText("");
    ui.confirm_password_edit->setText("");
}


void MainWindow::on_signup_signup_button_clicked()
{
    QString username = ui.username_edit->text();
    QString login = ui.login_edit->text();
    QString password = ui.password_edit->text();
    QString confirm_password = ui.confirm_password_edit->text();
    QString str_check = cmd->check_reg_form_task(username, login, password, confirm_password);
    if (str_check != "completed") {
        ui.signup_error_label->setText(str_check);
        return;
    }

    user.name = username;
    user.login = login;
    user.password = password;
    cmd->reg_user(user);
    if (!cmd->get_status()) {
        return;
    }
    ui.stackedWidget->setCurrentWidget(ui.SignIn);
    ui.signin_error_label->setStyleSheet("color: green;");
    ui.signin_error_label->setText("Пользователь успешно создан! Выполните вход");
    ui.signup_error_label->setText("");
    ui.username_edit->setText("");
    ui.login_edit->setText("");
    ui.password_edit->setText("");
    ui.confirm_password_edit->setText("");
}


void MainWindow::on_add_avatar_clicked()
{
     path_to_avatar = QFileDialog::getOpenFileName(this, "Выбор изображения", "/home", "Images (*.png *.xpm *.jpg *.jpeg)");
     if (path_to_avatar.isEmpty()) { return; }
     QPixmap avatar(path_to_avatar);
     int w = ui.signup_avatar->width();
     int h = ui.signup_avatar->height();
     ui.signup_avatar->setPixmap(avatar.scaled(w, h, Qt::KeepAspectRatio));
     ui.messendger_avatar->setPixmap(avatar.scaled(w, h, Qt::KeepAspectRatio));
     user.profile_avatar = path_to_avatar;
}


void MainWindow::on_delete_avatar_clicked()
{
    QPixmap avatar(":/Resources/profile.png");
    ui.signup_avatar->setPixmap(avatar);
    ui.messendger_avatar->setPixmap(avatar);
}




void MainWindow::on_add_button_clicked()
{
    if (ui.add_line->text() == "") { return; }
    Friend new_friend;
    new_friend.login = ui.add_line->text();
    if (new_friend.login.contains(" ")) { return; }
    cmd->find_friend(new_friend);
    if (!cmd->get_status()) {
        return;
    }
    ui.add_line->clear();
    ui.statusBar->clearMessage();
    friends.push_back(new_friend);
    ui.contacts->addItem(new_friend.name);
    contact.emplace(new_friend.name, new_friend);
}


void MainWindow::on_contact_button_clicked()
{
    Friend current_friend = contact.find(ui.contacts->currentItem()->text())->second;
    bool create_chat;
    QString text = QInputDialog::getText(this, tr("Создание чата"),
                                         tr("Название чата:"), QLineEdit::Normal,
                                            "", &create_chat);
    std::cout << create_chat << std::endl;
    if (create_chat && text.isEmpty()) { return; }
    chat.name = text;
    chat.users_id = {user.id, current_friend.id};
    cmd->create_chat(chat);
    if (!cmd->get_status()) {
        return;
    }
    ui.statusBar->clearMessage();
    ui.chat_windows->setCurrentWidget(ui.message_window);
    ui.chat_name_label->setText(chat.name);
    ui.message_window_friend_name->setText(contact.find(current_friend.name)->second.login);
    msg.hasAttachment = false;
}


void MainWindow::on_send_button_clicked()
{
    msg.text = ui.input_message_edit->toPlainText();
    ui.input_message_edit->clear();
    if (msg.text.isEmpty()) { return; }
    msg.senderId = user.id;
    msg.chatId = chat.id;
    if (!msg.hasAttachment) { msg.hasAttachment = ""; }
    cmd->send_message(msg);
    if (!cmd->get_status()) {
        return;
    }
    ui.statusBar->clearMessage();
    std::cout << msg.text.toStdString() << std::endl;
}


void MainWindow::on_add_image_button_clicked()
{
    QString path_to_image = QFileDialog::getOpenFileName(this, "Выбор изображения", "/home", "Images (*.png *.xpm *.jpg *.jpeg)");
    if (path_to_image.isEmpty()) {
        msg.hasAttachment = false;
        return;
    }
    msg.hasAttachment = true;
    msg.attachment = path_to_image;
}

void MainWindow::print_reg_error(QString reg_err)
{
    ui.signup_error_label->setText(reg_err);
}

void MainWindow::print_log_error(QString log_err)
{
    ui.signin_error_label->setText(log_err);
}

void MainWindow::print_find_friend_error(QString find_friend_error)
{
    ui.statusBar->clearMessage();
    ui.statusBar->showMessage(find_friend_error);
}

void MainWindow::print_create_chat_error(QString create_chat_error)
{
    ui.statusBar->clearMessage();
    ui.statusBar->showMessage(create_chat_error);
}

void MainWindow::print_send_message_error(QString send_message_error)
{
    ui.statusBar->clearMessage();
    ui.statusBar->showMessage(send_message_error);
}

void MainWindow::on_change_user_action_triggered()
{
    cmd->stop_connect();
    this->statusBar()->hide();
    this->menuBar()->hide();
    ui.stackedWidget->setCurrentWidget(ui.SignIn);
}


void MainWindow::on_exit_action_triggered()
{
    cmd->stop_connect();
    qApp->quit();
}
