#include <gtest/gtest.h>

#include "Exceptions.h"
#include "db_implementation.h"

// Перед запуском тестов необходимо запустить sql скрипт query.sql
// Данный скрипт инициализирует ДБ для тестирования
// TODO: сделать так, чтобы скрипт запускался автоматически

TEST(DBLibTest, AddUserValidInputs) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    UserForm userForm;
    User user;
    userForm.nickname = "User1453";
    user = postgres.AddUser(userForm);
    EXPECT_EQ(user.nickname, userForm.nickname);
    EXPECT_TRUE(user.profile_avatar.empty());
    EXPECT_FALSE(user.created_at.empty());
    EXPECT_TRUE(user.chats_id.empty());
    userForm.nickname = "User1454";
    userForm.profile_avatar = "/image/image.png";
    user = postgres.AddUser(userForm);
    EXPECT_EQ(user.nickname, userForm.nickname);
    EXPECT_EQ(user.profile_avatar, userForm.profile_avatar);
    EXPECT_FALSE(user.created_at.empty());
    EXPECT_TRUE(user.chats_id.empty());
}

TEST(DBLibTest, AddUserInvalidInputs) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    UserForm userForm;
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
    userForm.profile_avatar = "/image/image.png";
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
    userForm.nickname = "";
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
    userForm.nickname = "User1454";
    EXPECT_THROW(postgres.AddUser(userForm), pqxx::unique_violation);
}

TEST(DBLibTest, ExtractUserByIDTest) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    UserForm userForm;
    User user1, user2;
    userForm.nickname = "UserUser";
    userForm.profile_avatar = "/image/image.png";
    user1 = postgres.AddUser(userForm);
    user2 = postgres.ExtractUserByID(user1.id);
    EXPECT_EQ(user1.id, user2.id);
    EXPECT_EQ(user1.nickname, user2.nickname);
    EXPECT_EQ(user1.profile_avatar, user2.profile_avatar);
    EXPECT_EQ(user1.created_at, user2.created_at);
}

TEST(DBLibTest, ExtractUserByNickNameTest) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    UserForm userForm;
    User user1, user2;
    userForm.nickname = "UserTest4";
    userForm.profile_avatar = "/image/image.png";
    user1 = postgres.AddUser(userForm);
    user2 = postgres.ExtractUserByNickName(user1.nickname);
    EXPECT_EQ(user1.id, user2.id);
    EXPECT_EQ(user1.nickname, user2.nickname);
    EXPECT_EQ(user1.profile_avatar, user2.profile_avatar);
    EXPECT_EQ(user1.created_at, user2.created_at);
}

TEST(DBLibTest, AddChatValidInputs) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    ChatForm chatForm;
    Chat chat;
    std::vector<unsigned long> existingUsersID = {1, 2, 3, 4};
    chatForm.chat_name = "chatname";
    chatForm.users_id = existingUsersID;
    EXPECT_NO_THROW(chat = postgres.AddChat(chatForm));
    EXPECT_EQ(chatForm.chat_name, chat.chat_name);
    EXPECT_EQ(chatForm.users_id, chat.users_id);
}

TEST(DBLibTest, AddChatInvalidInputs) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    ChatForm chatForm;
    EXPECT_THROW(postgres.AddChat(chatForm), InvalidInputs);
    chatForm.chat_name = "123";
    EXPECT_THROW(postgres.AddChat(chatForm), InvalidInputs);
}

TEST(DBLibTest, ExtractChatByIDTest) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    std::vector<unsigned long> chatUsersID = {1, 2, 3, 4};
    unsigned long chatTotalMessages = 2;
    Chat chat;
    EXPECT_NO_THROW(chat = postgres.ExtractChatByID(1));
    EXPECT_EQ(chat.chat_name, "testChat");
    EXPECT_EQ(chat.users_id, chatUsersID);
    EXPECT_EQ(chat.created_at, "03-02-1998 13-15-07");
    EXPECT_EQ(chat.total_messages, chatTotalMessages);
    EXPECT_EQ(chat.id, 1);
}

TEST(DBLibTest, ExtractChatsIDByUserID) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    unsigned long UserIDTest = 5;
    std::vector<unsigned long> userChatsTest = {1, 2, 3};
    std::vector<unsigned long> userChats;
    EXPECT_NO_THROW(userChats = postgres.ExtractChatsIDByUserID(UserIDTest));
    EXPECT_EQ(userChatsTest, userChats);
}

TEST(DBLibTest, AddMessageValidInputs) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    MessageForm msgForm;
    message msg;
    msgForm.sender_id = 1;
    msgForm.chat_id = 1;
    msgForm.message = "msg";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ(msgForm.message, msg.message);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
    std::vector<std::string> att = {"attachment1", "attachment2"};
    msgForm.attachment = att;
    msgForm.message = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ(msgForm.message, msg.message);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
}

TEST(DBLibTest, AddMessageInvalidInputs) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    MessageForm msgForm;
    message msg;
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
    msgForm.sender_id = 1;
    msgForm.chat_id = 1;
    msgForm.message = "";
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
}

TEST(DBLibTest, ExtractChatMessagesID) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    unsigned long chatIDTest = 4;
    std::vector<unsigned long> msgID;
    std::vector<unsigned long> msgIDTest = {2, 3, 10};
    EXPECT_NO_THROW(msgID = postgres.ExtractChatMessagesID(chatIDTest, 0, 3));
    EXPECT_EQ(msgIDTest, msgID);
    EXPECT_THROW(msgID = postgres.ExtractChatMessagesID(chatIDTest, 0, 0),
                 InvalidInputs);
}

TEST(DBLibTest, ExtractMessageByID) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    unsigned long msgIDTest = 4;
    message msg, msgTest;
    msgTest.id = msgIDTest;
    msgTest.sender_id = 1;
    msgTest.chat_id = 2;
    msgTest.message = "TestMessage";
    msgTest.attachment = {"att1", "att2"};
    msgTest.created_at = "03-02-1998 13-15-07";
    msgTest.num_in_chat = 2;

    EXPECT_NO_THROW(msg = postgres.ExtractMessageByID(msgIDTest));
    EXPECT_EQ(msgTest.id, msg.id);
    EXPECT_EQ(msgTest.sender_id, msg.sender_id);
    EXPECT_EQ(msgTest.chat_id, msg.chat_id);
    EXPECT_EQ(msgTest.message, msg.message);
    EXPECT_EQ(msgTest.attachment, msg.attachment);
    EXPECT_EQ(msgTest.created_at, msg.created_at);
    EXPECT_EQ(msgTest.num_in_chat, msg.num_in_chat);
}

TEST(DBLibTest, LastMessagesByUserID) {
    PostgresDB postgres(
            "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg");
    unsigned long userIDTest = 4;
    std::vector<unsigned long> msgTest = {5, 7, 10};
    std::vector<unsigned long> msg;

    EXPECT_NO_THROW(msg = postgres.LastMessagesByUserID(userIDTest));
    EXPECT_EQ(msg, msgTest);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
