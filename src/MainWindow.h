#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QPixmap>
#include <QProcess>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <map>

#include "Commands.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
   signals:
    void start_form_output_msg(QString, Message);

   private slots:
    void on_signin_signup_button_clicked();

    void on_signup_haveAcc_button_clicked();

    void on_signup_signup_button_clicked();

    void on_signin_signin_button_clicked();

    void on_add_avatar_clicked();

    void on_delete_avatar_clicked();

    void on_add_button_clicked();

    void on_add_image_button_clicked();

    void print_reg_error(QString);

    void print_log_error(QString);

    void print_find_friend_error(QString);

    void print_create_chat_error(QString);

    void print_send_message_error(QString);

    void on_exit_action_triggered();

    void on_send_button_clicked();

    void form_message_output(QString, Message);

    void create_chats(const Chat);

    void itemDoubleClicked(QListWidgetItem *);

    void itemClicked(QListWidgetItem *);

    void on_update_chats_button_clicked();

    void on_update_current_chat_button_clicked();

   private:
    Ui::MainWindow ui;
    Commands *cmd;
    User user;
    std::vector<QListWidgetItem *> contact_items;
    std::map<QString, Chat> chat_by_friend_login;
    std::map<QListWidgetItem *, unsigned int> contact_to_layout;
    size_t index_page;
    QString path_to_avatar;
};

#endif  // MAINWINDOW_H
