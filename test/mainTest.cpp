#include <gtest/gtest.h>

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QtWidgets/QApplication>
#include <algorithm>
#include <cstdint>
#include <iostream>

#include "src/Chat.h"
#include "src/Client.h"
#include "src/Commands.h"
#include "src/MainWindow.h"
#include "src/Message.h"
#include "src/User.h"

TEST(USER_TEST, EQ) {
    User user;
    user.setId(1);
    user.setName("Arkady");
    user.setStatus("Online");
    user.setLogin("Arcades");
    EXPECT_EQ(user.getName(), "Arkady");
    EXPECT_EQ(user.getLogin(), "Arcades");
    EXPECT_EQ(user.getStatus(), "Online");
    EXPECT_EQ(user.getId(), 1);
}

TEST(CHAT_TEST, EQ) {
    Chat chat;
    chat.setId(1);
    chat.setName("Class");
    chat.setCreatorId(2);
    EXPECT_EQ(chat.getName(), "Class");
    EXPECT_EQ(chat.getCreatorId(), 2);
    EXPECT_EQ(chat.getId(), 1);
}

TEST(MESSAGE_TEST, EQ) {
    Message msg;
    msg.setId(1);
    msg.setSenderId(2);
    msg.setText("Valhalla");
    msg.setChatId(1);
    msg.setHasAttachment(0);
    EXPECT_EQ(msg.getSenderId(), 2);
    EXPECT_EQ(msg.getText(), "Valhalla");
    EXPECT_EQ(msg.getChatId(), 1);
    EXPECT_EQ(msg.getId(), 1);
    EXPECT_EQ(msg.getHasAttachment(), 0);
}

TEST(CLIENT_TEST, START) {
    auto const host = "0.0.0.0";
    auto const port = "5000";
    auto const text = "Hello, server!";
    net::io_context ioc;
    std::make_shared<Client>(ioc)->run(host, port, text);
    ioc.run();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
