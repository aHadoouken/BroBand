#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QProcess>
#include <QKeyEvent>
#include <QFileDialog>
#include <QDir>
#include <QPixmap>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QObject>
#include <QInputDialog>
#include <map>

#include "Commands.h"
#include "ui_MainWindow.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_signin_signup_button_clicked();

    void on_signup_haveAcc_button_clicked();

    void on_signup_signup_button_clicked();

    void on_signin_signin_button_clicked();

    void on_add_avatar_clicked();

    void on_delete_avatar_clicked();

    void on_add_button_clicked();

    void on_contact_button_clicked();

    void on_add_image_button_clicked();

    void print_reg_error(QString);

    void print_log_error(QString);

    void print_find_friend_error(QString);

    void print_create_chat_error(QString);

    void print_send_message_error(QString);

    void on_change_user_action_triggered();

    void on_exit_action_triggered();

    void on_send_button_clicked();

private:
    Ui::MainWindow ui;
    Commands *cmd;
    User user;
    Chat chat;
    Message msg;
    std::vector<Friend> friends;
    std::map <QString, Friend> contact;

    QString path_to_avatar;


};

#endif  // MAINWINDOW_H
