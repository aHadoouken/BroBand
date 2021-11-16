#include <gtest/gtest.h>

#include "db_implementation.h"

TEST(DBLibTest, AddUserValidInputs) {
    PostgresDB postgres;
    struct UserForm userForm;
    struct User user;
    // В качестве ника выбирается время, чтобы не задавался существующий ник
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "1%d-%m-%Y %H-%M-%S");

    userForm.NickName = oss.str();
    user = postgres.AddUser(userForm);
    EXPECT_EQ(user.NickName, userForm.NickName);
    oss << std::put_time(&tm, "%2d-%m-%Y %H-%M-%S");
    userForm.NickName = oss.str();
    userForm.ProfileAvatar = "/image/image.png";
    user = postgres.AddUser(userForm);
    EXPECT_EQ(user.NickName, userForm.NickName);
    EXPECT_EQ(user.ProfileAvatar, userForm.ProfileAvatar);
}

TEST(DBLibTest, AddUserInvalidInputs) {
    PostgresDB postgres;
    struct UserForm userForm;
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
    userForm.ProfileAvatar = "/image/image.png";
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
    userForm.NickName = "";
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
}

TEST(DBLibTest, ExtractUserByIDTest) {
    PostgresDB postgres;
    struct UserForm userForm;
    struct User user1, user2;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%3d-%m-%Y %H-%M-%S");

    userForm.NickName = oss.str();
    userForm.ProfileAvatar = "/image/image.png";
    user1 = postgres.AddUser(userForm);
    user2 = postgres.ExtractUserByID(user1.id);
    EXPECT_EQ(user1.id, user2.id);
    EXPECT_EQ(user1.NickName, user2.NickName);
    EXPECT_EQ(user1.ProfileAvatar, user2.ProfileAvatar);
    EXPECT_EQ(user1.CreatedAt, user2.CreatedAt);
}

TEST(DBLibTest, ExtractUserByNickNameTest) {
    PostgresDB postgres;
    struct UserForm userForm;
    struct User user1, user2;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%4d-%m-%Y %H-%M-%S");

    userForm.NickName = oss.str();
    userForm.ProfileAvatar = "/image/image.png";
    user1 = postgres.AddUser(userForm);
    user2 = postgres.ExtractUserByNickName(user1.NickName);
    EXPECT_EQ(user1.id, user2.id);
    EXPECT_EQ(user1.NickName, user2.NickName);
    EXPECT_EQ(user1.ProfileAvatar, user2.ProfileAvatar);
    EXPECT_EQ(user1.CreatedAt, user2.CreatedAt);
}

// Далее предполагается что все необходимые значения уже хранятся в БД
// TODO: реализовать скрипт, который формирует базу данных для тестов
TEST(DBLibTest, AddChatValidInputs) {
    PostgresDB postgres;
    struct ChatForm chatForm;
    struct Chat chat;
    std::vector<int> existingUsersID = {1, 2, 3, 4};
    chatForm.ChatName = "chatname";
    chatForm.UsersID = existingUsersID;
    EXPECT_NO_THROW(chat = postgres.AddChat(chatForm));
    EXPECT_EQ(chatForm.ChatName, chat.ChatName);
    EXPECT_EQ(chatForm.UsersID, chat.UsersID);
}

TEST(DBLibTest, AddChatInvalidInputs) {
    PostgresDB postgres;
    struct ChatForm chatForm;
    EXPECT_THROW(postgres.AddChat(chatForm), InvalidInputs);
    chatForm.ChatName = "123";
    EXPECT_THROW(postgres.AddChat(chatForm), InvalidInputs);
}

TEST(DBLibTest, ExtractChatByIDTest) {
    PostgresDB postgres;
    std::vector<int> chatUsersID = {1, 2, 3, 4};
    int chatTotalMessages = 2;
    Chat chat;
    EXPECT_NO_THROW(chat = postgres.ExtractChatByID(1));
    EXPECT_EQ(chat.ChatName, "testChat");
    EXPECT_EQ(chat.UsersID, chatUsersID);
    EXPECT_EQ(chat.CreatedAt, "03-02-1998 13-15-07");
    EXPECT_EQ(chat.TotalMessages, chatTotalMessages);
    EXPECT_EQ(chat.id, 1);
}

TEST(DBLibTest, ExtractChatsIDByUserID) {
    PostgresDB postgres;
    int UserIDTest = 5;
    std::vector<int> userChatsTest = {1, 2, 3};
    std::vector<int> userChats;
    EXPECT_NO_THROW(userChats = postgres.ExtractChatsIDByUserID(UserIDTest));
    EXPECT_EQ(userChatsTest, userChats);
}

TEST(DBLibTest, AddMessageValidInputs) {
    PostgresDB postgres;
    MessageForm msgForm;
    Message msg;
    msgForm.SenderID = 1;
    msgForm.ChatID = 1;
    msgForm.Message = "msg";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.SenderID, msg.SenderID);
    EXPECT_EQ(msgForm.ChatID, msg.ChatID);
    EXPECT_EQ(msgForm.Message, msg.Message);
    EXPECT_EQ(msgForm.Attachment, msg.Attachment);
    std::vector<std::string> att = {"attachment1", "attachment2"};
    msgForm.Attachment = att;
    msgForm.Message = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.SenderID, msg.SenderID);
    EXPECT_EQ(msgForm.ChatID, msg.ChatID);
    EXPECT_EQ(msgForm.Message, msg.Message);
    EXPECT_EQ(msgForm.Attachment, msg.Attachment);
}

TEST(DBLibTest, AddMessageInvalidInputs) {
    PostgresDB postgres;
    MessageForm msgForm;
    Message msg;
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
    msgForm.SenderID = 1;
    msgForm.ChatID = 1;
    msgForm.Message = "";
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
}

TEST(DBLibTest, ExtractChatMessagesID) {
    PostgresDB postgres;
    int chatIDTest = 4;
    std::vector<int> msgID;
    std::vector<int> msgIDTest = {2, 3, 10};
    EXPECT_NO_THROW(msgID = postgres.ExtractChatMessagesID(chatIDTest, 0, 3));
    EXPECT_EQ(msgIDTest, msgID);
    EXPECT_THROW(msgID = postgres.ExtractChatMessagesID(chatIDTest, 0, 0),
                 InvalidInputs);
}

TEST(DBLibTest, ExtractMessageByID) {
    PostgresDB postgres;
    int msgIDTest = 4;
    Message msg, msgTest;
    msgTest.id = msgIDTest;
    msgTest.SenderID = 1;
    msgTest.ChatID = 2;
    msgTest.Message = "TestMessage";
    msgTest.Attachment = {"att1", "att2"};
    msgTest.CreatedAt = "03-02-1998 13-15-07";
    msgTest.NumInChat = 2;

    EXPECT_NO_THROW(msg = postgres.ExtractMessageByID(msgIDTest));
    EXPECT_EQ(msgTest.id, msg.id);
    EXPECT_EQ(msgTest.SenderID, msg.SenderID);
    EXPECT_EQ(msgTest.ChatID, msg.ChatID);
    EXPECT_EQ(msgTest.Message, msg.Message);
    EXPECT_EQ(msgTest.Attachment, msg.Attachment);
    EXPECT_EQ(msgTest.CreatedAt, msg.CreatedAt);
    EXPECT_EQ(msgTest.NumInChat, msg.NumInChat);
}

TEST(DBLibTest, LastMessagesByUserID) {
    PostgresDB postgres;
    int userIDTest = 4;
    std::vector<int> msgTest = {5, 7, 10};
    std::vector<int> msg;

    EXPECT_NO_THROW(msg = postgres.LastMessagesByUserID(userIDTest));
    EXPECT_EQ(msg, msgTest);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
