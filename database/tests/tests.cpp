#include <gtest/gtest.h>

#include "Exceptions.h"
#include "db_implementation.h"

#define CONFIG "/home/alex/mail_cpp/BroBand/database/tests/db_config.cfg"

// Перед запуском тестов необходимо запустить sql скрипт query.sql
// Данный скрипт инициализирует ДБ для тестирования
// TODO: сделать так, чтобы скрипт запускался автоматически

TEST(DBLibTest, AddUserValidInputs) {
    PostgresDB postgres(CONFIG);
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
    PostgresDB postgres(CONFIG);
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
    PostgresDB postgres(CONFIG);
    User user;
    user = postgres.ExtractUserByID(1);
    EXPECT_EQ(user.id, 1);
    EXPECT_EQ(user.nickname, "user1");
    EXPECT_TRUE(user.profile_avatar.empty());
    std::vector<unsigned long> chats_id = {1, 2, 3};
    EXPECT_EQ(user.chats_id, chats_id);
    EXPECT_THROW(postgres.ExtractUserByID(42), pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, ExtractUserByNickNameTest) {
    PostgresDB postgres(CONFIG);
    User user;
    user = postgres.ExtractUserByNickName("user1");
    EXPECT_EQ(user.id, 1);
    EXPECT_EQ(user.nickname, "user1");
    EXPECT_TRUE(user.profile_avatar.empty());
    std::vector<unsigned long> chats_id = {1, 2, 3};
    EXPECT_EQ(user.chats_id, chats_id);
    EXPECT_THROW(postgres.ExtractUserByNickName("user144"),
                 pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, AddChatValidInputs) {
    PostgresDB postgres(CONFIG);
    ChatForm chatForm;
    Chat chat;
    std::vector<unsigned long> existingUsersID = {1, 2, 3};
    chatForm.chat_name = "chatname";
    chatForm.users_id = existingUsersID;
    EXPECT_NO_THROW(chat = postgres.AddChat(chatForm));
    EXPECT_EQ(chatForm.chat_name, chat.chat_name);
    EXPECT_EQ(chatForm.users_id, chat.users_id);
}

TEST(DBLibTest, AddChatInvalidInputs) {
    PostgresDB postgres(CONFIG);
    ChatForm chatForm;
    EXPECT_THROW(postgres.AddChat(chatForm), InvalidInputs);
    chatForm.chat_name = "123";
    EXPECT_THROW(postgres.AddChat(chatForm), InvalidInputs);
    chatForm.users_id = {456, 478};
    EXPECT_THROW(postgres.AddChat(chatForm), pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, ExtractChatByIDTest) {
    PostgresDB postgres(CONFIG);
    Chat chat;
    chat = postgres.ExtractChatByID(1);
    EXPECT_EQ(chat.id, 1);
    EXPECT_EQ(chat.chat_name, "chat1");
    EXPECT_EQ(chat.total_messages, 10);
    std::vector<unsigned long> users_id = {1, 2};
    EXPECT_EQ(chat.users_id, users_id);
    EXPECT_THROW(postgres.ExtractUserByID(402), pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, AddMessageValidInputs) {
    PostgresDB postgres(CONFIG);
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
    PostgresDB postgres(CONFIG);
    MessageForm msgForm;
    message msg;
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
    msgForm.sender_id = 1;
    msgForm.chat_id = 1;
    msgForm.message = "";
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
}

TEST(DBLibTest, ExtractChatMessagesID) {
    PostgresDB postgres(CONFIG);
    unsigned long chatIDTest = 4;
    std::vector<unsigned long> msgID;
    std::vector<unsigned long> msgIDTest = {2, 3, 10};
    EXPECT_NO_THROW(msgID = postgres.ExtractChatMessagesID(chatIDTest, 0, 3));
    EXPECT_EQ(msgIDTest, msgID);
    EXPECT_THROW(msgID = postgres.ExtractChatMessagesID(chatIDTest, 0, 0),
                 InvalidInputs);
}

TEST(DBLibTest, ExtractMessageByID) {
    PostgresDB postgres(CONFIG);
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
    PostgresDB postgres(CONFIG);
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
