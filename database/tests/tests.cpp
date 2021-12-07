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
    userForm.login = "User1453";
    userForm.name = "name1453";
    userForm.password = "password1453";
    auto user = postgres.AddUser(userForm);
    EXPECT_EQ(user.login, userForm.login);
    EXPECT_EQ(user.name, userForm.name);
    EXPECT_TRUE(user.profile_avatar.empty());
    EXPECT_FALSE(user.created_at.empty());
    EXPECT_TRUE(user.chats_id.empty());
    userForm.login = "User1454";
    userForm.profile_avatar = "/image/image.png";
    user = postgres.AddUser(userForm);
    EXPECT_EQ(user.login, userForm.login);
    EXPECT_EQ(user.name, userForm.name);
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
    userForm.login = "";
    EXPECT_THROW(postgres.AddUser(userForm), InvalidInputs);
    userForm.login = "User1454";
    userForm.name = "name1453";
    userForm.password = "password1453";
    EXPECT_THROW(postgres.AddUser(userForm), pqxx::unique_violation);
}

TEST(DBLibTest, GetUserByIDTest) {
    PostgresDB postgres(CONFIG);
    User user;
    user = postgres.GetUserByID(1);
    EXPECT_EQ(user.id, 1);
    EXPECT_EQ(user.login, "user1");
    EXPECT_TRUE(user.profile_avatar.empty());
    std::vector<unsigned long> chats_id = {1, 2, 3};
    EXPECT_EQ(user.chats_id, chats_id);
    EXPECT_EQ(user.name, "name1");
    EXPECT_THROW(postgres.GetUserByID(1142), pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, GetUserByLoginTest) {
    PostgresDB postgres(CONFIG);
    User user;
    user = postgres.GetUserByLogin("user1");
    EXPECT_EQ(user.id, 1);
    EXPECT_EQ(user.login, "user1");
    EXPECT_TRUE(user.profile_avatar.empty());
    std::vector<unsigned long> chats_id = {1, 2, 3};
    EXPECT_EQ(user.chats_id, chats_id);
    EXPECT_EQ(user.name, "name1");
    EXPECT_THROW(postgres.GetUserByLogin("user1444444"),
                 pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, AuthorizationTest) {
    PostgresDB postgres(CONFIG);
    UserLogin userLogin;
    userLogin.login = "user1";
    userLogin.password = "password1";
    EXPECT_TRUE(postgres.Authorization(userLogin));
    userLogin.password = "password122";
    EXPECT_FALSE(postgres.Authorization(userLogin));
    userLogin.login = "user1123123123";
    EXPECT_THROW(postgres.Authorization(userLogin),
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

TEST(DBLibTest, GetChatByIDTest) {
    PostgresDB postgres(CONFIG);
    Chat chat;
    chat = postgres.GetChatByID(1);
    EXPECT_EQ(chat.id, 1);
    EXPECT_EQ(chat.chat_name, "chat1");
    EXPECT_EQ(chat.total_messages, 10);
    std::vector<unsigned long> users_id = {1, 2};
    EXPECT_EQ(chat.users_id, users_id);
    EXPECT_THROW(postgres.GetUserByID(402), pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, AddMessageValidInputs) {
    PostgresDB postgres(CONFIG);
    MessageForm msgForm;
    Message msg;
    msgForm.sender_id = 1;
    msgForm.chat_id = 1;
    msgForm.text = "msg";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ(msgForm.text, msg.text);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
    msgForm.attachment = "attachment";
    msgForm.text = "";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ(msgForm.text, msg.text);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
    msgForm.sender_id = 4;
    msgForm.chat_id = 2;
    msgForm.attachment = "attachment";
    msgForm.text = "msg2";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ(msgForm.text, msg.text);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
}

TEST(DBLibTest, AddMessageInvalidInputs) {
    PostgresDB postgres(CONFIG);
    MessageForm msgForm;
    Message msg;
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
    msgForm.sender_id = 1;
    msgForm.chat_id = 1;
    msgForm.text = "";
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
    msgForm.sender_id = 1000222;
    msgForm.text = "123123";
    EXPECT_THROW(msg = postgres.AddMessage(msgForm),
                 pqxx::plpgsql_no_data_found);
    msgForm.sender_id = 3;
    msgForm.chat_id = 1;
    EXPECT_THROW(msg = postgres.AddMessage(msgForm), InvalidInputs);
    msgForm.chat_id = 10000;
    EXPECT_THROW(msg = postgres.AddMessage(msgForm),
                 pqxx::plpgsql_no_data_found);
}

TEST(DBLibTest, GetChatMessages) {
    PostgresDB postgres(CONFIG);
    unsigned long chatIDTest = 4;
    std::vector<Message> msgs;
    EXPECT_NO_THROW(msgs = postgres.GetChatMessages(1));
    EXPECT_EQ(msgs[0].id, 1);
    EXPECT_EQ(msgs[0].sender_id, 1);
    EXPECT_EQ(msgs[0].chat_id, 1);
    EXPECT_EQ(msgs[0].text, "message1 1");
    EXPECT_EQ(msgs[0].attachment, "/image/1_1");
    EXPECT_EQ(msgs[1].id, 2);
    EXPECT_EQ(msgs[1].sender_id, 2);
    EXPECT_EQ(msgs[1].chat_id, 1);
    EXPECT_TRUE(msgs[1].text.empty());
    EXPECT_EQ(msgs[1].attachment, "/image/2_1");
}

TEST(DBLibTest, GetChatMessagesID) {
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
    Message msg, msgTest;
    msgTest.id = msgIDTest;
    msgTest.sender_id = 1;
    msgTest.chat_id = 2;
    msgTest.text = "TestMessage";
    msgTest.attachment = {"att1", "att2"};
    msgTest.created_at = "03-02-1998 13-15-07";

    EXPECT_NO_THROW(msg = postgres.ExtractMessageByID(msgIDTest));
    EXPECT_EQ(msgTest.id, msg.id);
    EXPECT_EQ(msgTest.sender_id, msg.sender_id);
    EXPECT_EQ(msgTest.chat_id, msg.chat_id);
    EXPECT_EQ(msgTest.text, msg.text);
    EXPECT_EQ(msgTest.attachment, msg.attachment);
    EXPECT_EQ(msgTest.created_at, msg.created_at);
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
