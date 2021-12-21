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
    index_page = ui.chat_windows->currentIndex();
    ui.input_frame->hide();

    cmd = new Commands();
    cmd->start();

    connect(cmd, SIGNAL(reg_error_gen(QString)), this, SLOT(print_reg_error(QString)));
    connect(cmd, SIGNAL(log_error_gen(QString)), this, SLOT(print_log_error(QString)));
    connect(cmd, SIGNAL(create_chat_error_gen(QString)), this, SLOT(print_create_chat_error(QString)));
    connect(cmd, SIGNAL(send_message_error_gen(QString)), this, SLOT(print_send_message_error(QString)));
    connect(cmd, SIGNAL(find_friend_error_gen(QString)), this, SLOT(print_find_friend_error(QString)));
    connect(cmd, SIGNAL(start_create_chat(Chat)), this, SLOT(create_chats(Chat)));
    connect(this, SIGNAL(start_form_output_msg(QString, Message)), this,
            SLOT(form_message_output(QString, Message)));
    connect(cmd, SIGNAL(start_form_new_msg(QString, Message)), this,
            SLOT(form_message_output(QString, Message)));
    connect(ui.contacts, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(itemDoubleClicked(QListWidgetItem *)));
    connect(ui.contacts, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

MainWindow::~MainWindow() {
    cmd->disconnect();
    delete cmd;
}

void MainWindow::on_signin_signup_button_clicked() {
    ui.stackedWidget->setCurrentWidget(ui.SignUp);
    ui.signin_error_label->setStyleSheet("color: rgb(239, 41, 41);");
}

void MainWindow::on_signin_signin_button_clicked() {
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
    ui.username_label->setText(user.name);
    ui.status_connection_label->setText(user.status);
    if (!user.profile_avatar.isEmpty()) {
        QPixmap avatar(user.profile_avatar);
        int w = ui.messendger_avatar->width();
        int h = ui.messendger_avatar->height();
        ui.messendger_avatar->setPixmap(avatar.scaled(w, h, Qt::KeepAspectRatio));
    } else {
        QPixmap avatar(":/Resources/profile.png");
        ui.signup_avatar->setPixmap(avatar);
    }
    if (!user.chats_id.empty()) {
        for (size_t i = 0; i < user.chats_id.size(); ++i) {
            Chat old_chat;
            old_chat.id = user.chats_id.at(i);
            cmd->get_user_chat(old_chat, user);
            chat_by_friend_login.begin()->second = user.user_chats_by_id.find(old_chat.id)->second;
        }
    }
    ui.chat_windows->setCurrentWidget(ui.empty);
    ui.stackedWidget->setCurrentWidget(ui.Messanger);
    this->menuBar()->show();
    this->statusBar()->show();
    ui.signin_error_label->setText("");
    ui.signin_login_edit->setText("");
    ui.signin_password_edit->setText("");
}

void MainWindow::create_chats(Chat chat) {
    for (size_t i = 0; i < chat.users_id.size(); ++i) {
        if (chat.users_id.at(i) != user.id) {
            Friend fr;
            fr = user.user_friend_by_id.find(chat.users_id.at(i))->second;
            QPixmap img(fr.profile_avatar);
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(img);
            item->setText(fr.name);
            ui.contacts->addItem(item);
            contact_items.push_back(item);
            index_page++;
            contact_to_layout.emplace(item, index_page);
            chat_by_friend_login.emplace(fr.login, chat);
            ui.chat_windows->setCurrentIndex(index_page++);
        }
    }
}

void MainWindow::itemDoubleClicked(QListWidgetItem *) {
    ui.statusBar->clearMessage();
    Friend current_friend = user.user_friend_by_name.find(ui.contacts->currentItem()->text())->second;
    if (chat_by_friend_login.find(current_friend.login) != chat_by_friend_login.end()) {
        return;
    }
    bool create_chat;
    QString text = QInputDialog::getText(this, tr("Создание чата"), tr("Название чата:"), QLineEdit::Normal,
                                         "", &create_chat);
    if (create_chat && text.isEmpty()) {
        return;
    }
    Chat new_chat;
    new_chat.name = text;
    new_chat.users_id = {user.id, current_friend.id};
    cmd->create_chat(new_chat);
    if (!cmd->get_status()) {
        return;
    }

    new_chat.last_msg_id = 0;
    new_chat.last_sender_id = 0;

    user.user_chats_by_name.emplace(new_chat.name, new_chat);
    user.user_chats_by_id.emplace(new_chat.id, new_chat);

    chat_by_friend_login.emplace(current_friend.login, new_chat);

    index_page++;

    contact_to_layout.emplace(ui.contacts->currentItem(), index_page);

    ui.chat_windows->setCurrentIndex(index_page);
    ui.input_frame->show();
    ui.chat_name_label->setText(new_chat.name);
    ui.message_window_friend_login->setText(current_friend.login);
}

void MainWindow::itemClicked(QListWidgetItem *) {
    ui.statusBar->clearMessage();
    Friend current_friend = user.user_friend_by_name.find(ui.contacts->currentItem()->text())->second;
    if (chat_by_friend_login.find(current_friend.login) == chat_by_friend_login.end()) {
        ui.input_frame->hide();
        ui.chat_windows->setCurrentWidget(ui.empty);
        ui.chat_name_label->clear();
        ui.message_window_friend_login->clear();
        return;
    }
    ui.input_frame->show();
    Chat chat = chat_by_friend_login.find(current_friend.login)->second;
    ui.chat_name_label->setText(chat.name);
    ui.message_window_friend_login->setText(current_friend.login);
    ui.chat_windows->setCurrentIndex(contact_to_layout.find(ui.contacts->currentItem())->second);
}

void MainWindow::on_signup_haveAcc_button_clicked() {
    ui.signin_error_label->setText("");
    ui.stackedWidget->setCurrentWidget(ui.SignIn);
    ui.signup_error_label->setText("");
    ui.username_edit->setText("");
    ui.login_edit->setText("");
    ui.password_edit->setText("");
    ui.confirm_password_edit->setText("");
    QPixmap avatar(":/Resources/profile.png");
    ui.signup_avatar->setPixmap(avatar);
}

void MainWindow::on_signup_signup_button_clicked() {
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
    QPixmap avatar(":/Resources/profile.png");
    ui.signup_avatar->setPixmap(avatar);
}

void MainWindow::on_add_avatar_clicked() {
    path_to_avatar =
        QFileDialog::getOpenFileName(this, "Выбор изображения", "/home", "Images (*.png *.xpm *.jpg *.jpeg)");
    if (path_to_avatar.isEmpty()) {
        return;
    }
    QPixmap avatar(path_to_avatar);
    int w = ui.signup_avatar->width();
    int h = ui.signup_avatar->height();
    ui.signup_avatar->setPixmap(avatar.scaled(w, h, Qt::KeepAspectRatio));
    user.profile_avatar = path_to_avatar;
}

void MainWindow::on_delete_avatar_clicked() {
    QPixmap avatar(":/Resources/profile.png");
    ui.signup_avatar->setPixmap(avatar);
}

void MainWindow::on_add_button_clicked() {
    ui.statusBar->clearMessage();
    if (ui.add_line->text().isEmpty()) {
        return;
    }
    Friend new_friend;
    new_friend.login = ui.add_line->text();
    if (user.user_friend_by_login.find(new_friend.login) != user.user_friend_by_login.end()) {
        ui.statusBar->showMessage("Пользователь уже был добавлен в в список контактов");
        ui.add_line->clear();
        ui.add_line->setFocus();
        return;
    }
    if (new_friend.login.contains(" ")) {
        return;
    }
    cmd->find_friend(new_friend);
    if (!cmd->get_status()) {
        ui.add_line->clear();
        ui.add_line->setFocus();
        return;
    }
    ui.add_line->clear();
    ui.add_line->setFocus();
    QPixmap img(new_friend.profile_avatar);
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(img);
    item->setText(new_friend.name);
    ui.contacts->addItem(item);

    contact_items.push_back(item);

    user.user_friend_by_name.emplace(new_friend.name, new_friend);
    user.user_friend_by_login.emplace(new_friend.login, new_friend);
    user.user_friend_by_id.emplace(new_friend.id, new_friend);
}

void MainWindow::on_send_button_clicked() {
    ui.statusBar->clearMessage();
    Friend current_friend = user.user_friend_by_name.find(ui.contacts->currentItem()->text())->second;
    Chat chat = chat_by_friend_login.find(current_friend.login)->second;

    user.msg.text = ui.input_message_edit->toPlainText();
    ui.input_message_edit->clear();
    if (user.msg.text.isEmpty() && !user.msg.hasAttachment) {
        return;
    }
    user.msg.senderId = user.id;
    user.msg.chatId = chat.id;
    if (!user.msg.hasAttachment) {
        user.msg.attachment = "";
    }
    cmd->send_message(user.msg);
    if (!cmd->get_status()) {
        return;
    }
    user.msg.hasAttachment = false;
    user.msg.attachment.clear();
}

void MainWindow::form_message_output(QString mode, Message n_msg) {
    const QString user_style =
        "background: rgb(52, 101, 164);"
        "border-radius: 5px;"
        "color: rgb(211, 215, 207);;"
        "font:10pt \"Open Sans\";";

    const QString time_style =
        "background: rgb(23, 43, 74);"
        "color: white;"
        "font:8pt \"Open Sans\";";

    const QString friend_style =
        "background: #4A4A86;"
        "border-radius: 5px;"
        "color: rgb(211, 215, 207);;"
        "font:10pt \"Open Sans\";";
    QHBoxLayout *layout_h = new QHBoxLayout;
    layout_h->setSpacing(10);
    QVBoxLayout *layout_v = new QVBoxLayout;
    QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
    QVBoxLayout *current_window = static_cast<QVBoxLayout *>(ui.chat_windows->currentWidget()
                                                                 ->children()
                                                                 .at(1)
                                                                 ->children()
                                                                 .at(0)
                                                                 ->children()
                                                                 .at(0)
                                                                 ->children()
                                                                 .at(0)
                                                                 ->children()
                                                                 .at(0));
    QLabel *message_label = new QLabel();
    message_label->setMargin(10);
    message_label->setWordWrap(true);
    QByteArray new_message;
    QTime ct = QTime::currentTime();
    QLabel *time_msg = new QLabel();
    time_msg->setMaximumHeight(10);
    time_msg->setStyleSheet(time_style);
    time_msg->setText(ct.toString());
    time_msg->setMargin(0);
    QLabel *space = new QLabel();
    space->setFixedSize(1, 1);

    int line_break_count = 0;
    int max_line_length = 30;
    int count_warp = 0;

    for (int i = 0; i < n_msg.text.length(); i++) {
        if (n_msg.text.at(i) > 32) {
            new_message += n_msg.text.at(i);
            line_break_count++;
        }
        if (n_msg.text.at(i) == 32) {
            new_message += n_msg.text.at(i);
            line_break_count = 0;
        }
        if (n_msg.text.at(i) == '\n') {
            new_message += '\n';
            line_break_count = 0;
            count_warp++;
        }
        if (line_break_count == max_line_length) {
            new_message += '\n';
            line_break_count = 0;
            count_warp++;
        }
    }

    if (n_msg.hasAttachment) {
        QPixmap img(n_msg.attachment);

        message_label->setMaximumSize(300, 200);
        int w = message_label->width();
        int h = message_label->height();
        message_label->setPixmap(img.scaled(w, h, Qt::KeepAspectRatio));
    }

    else {
        message_label->setMaximumWidth(400);
        message_label->setText(new_message);
    }

    if (mode == "user") {
        message_label->setStyleSheet(user_style);
        layout_v->addWidget(message_label, 0, Qt::AlignRight);
        layout_v->addWidget(time_msg, 0, Qt::AlignRight);
        layout_h->addSpacerItem(spacer);
        layout_h->addLayout(layout_v);
        layout_h->addWidget(space, 0);
        current_window->addLayout(layout_h);
    } else {
        message_label->setStyleSheet(friend_style);
        layout_v->addWidget(message_label, 0, Qt::AlignLeft);
        layout_v->addWidget(time_msg, 0, Qt::AlignLeft);
        layout_h->addLayout(layout_v);
        layout_h->addWidget(space, 0);
        layout_h->addSpacerItem(spacer);
        current_window->addLayout(layout_h);
    }
    ui.input_message_edit->clear();
    ui.input_message_edit->setFocus();
}

void MainWindow::on_add_image_button_clicked() {
    QString path_to_image =
        QFileDialog::getOpenFileName(this, "Выбор изображения", "/home", "Images (*.png *.xpm *.jpg *.jpeg)");
    if (path_to_image.isEmpty()) {
        user.msg.hasAttachment = false;
        return;
    }
    user.msg.hasAttachment = true;
    user.msg.attachment = path_to_image;
}

void MainWindow::print_reg_error(QString reg_err) { ui.signup_error_label->setText(reg_err); }

void MainWindow::print_log_error(QString log_err) { ui.signin_error_label->setText(log_err); }

void MainWindow::print_find_friend_error(QString find_friend_error) {
    ui.statusBar->clearMessage();
    ui.statusBar->showMessage(find_friend_error);
}

void MainWindow::print_create_chat_error(QString create_chat_error) {
    ui.statusBar->clearMessage();
    ui.statusBar->showMessage(create_chat_error);
}

void MainWindow::print_send_message_error(QString send_message_error) {
    ui.statusBar->clearMessage();
    ui.statusBar->showMessage(send_message_error);
}

void MainWindow::on_exit_action_triggered() {
    cmd->stop_connect();
    qApp->quit();
}

void MainWindow::on_update_chats_button_clicked() {
    cmd->update_chats(user);
    if (!cmd->get_status()) {
        return;
    }
    ui.chat_windows->setCurrentWidget(ui.empty);
}

void MainWindow::on_update_current_chat_button_clicked() {
    Chat chat = user.user_chats_by_name.find(ui.chat_name_label->text())->second;
    cmd->update_current_chat(chat, user);
    if (!cmd->get_status()) {
        return;
    }
}
