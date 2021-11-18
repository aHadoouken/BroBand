#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Chat.h"
#include "Client.h"
#include "Commands.h"
#include "Message.h"
#include "User.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private:
    Ui::MainWindow *ui;
    Commands *cmd;
    Client *client;
    User user;
    Chat chat;
    Message msg;
};

#endif  // MAINWINDOW_H
