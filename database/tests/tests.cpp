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

TEST(DBLibTest, GetPassword) {
    PostgresDB postgres(CONFIG);
    std::string login = "user1";
    std::string real_password = "password1";
    std::string password;
    EXPECT_NO_THROW(password = postgres.GetPassword(login));
    EXPECT_EQ(password, real_password);
    login = "user1123123123";
    EXPECT_THROW(postgres.GetPassword(login),
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
    EXPECT_EQ("name1", msg.sender_name);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ("chat1", msg.chat_name);
    EXPECT_EQ(msgForm.text, msg.text);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
    msgForm.attachment = "attachment";
    msgForm.text = "";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ("name1", msg.sender_name);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ("chat1", msg.chat_name);
    EXPECT_EQ(msgForm.text, msg.text);
    EXPECT_EQ(msgForm.attachment, msg.attachment);
    msgForm.sender_id = 4;
    msgForm.chat_id = 2;
    msgForm.attachment = "attachment";
    msgForm.text = "msg2";
    EXPECT_NO_THROW(msg = postgres.AddMessage(msgForm));
    EXPECT_EQ(msgForm.sender_id, msg.sender_id);
    EXPECT_EQ("name4", msg.sender_name);
    EXPECT_EQ(msgForm.chat_id, msg.chat_id);
    EXPECT_EQ("chat2", msg.chat_name);
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
    unsigned long chatIDTest = 1;
    std::vector<Message> msgs;
    EXPECT_NO_THROW(msgs = postgres.GetChatMessages(chatIDTest));
    EXPECT_EQ(msgs[0].id, 1);
    EXPECT_EQ(msgs[0].sender_id, 1);
    EXPECT_EQ(msgs[0].sender_name, "name1");
    EXPECT_EQ(msgs[0].chat_id, chatIDTest);
    EXPECT_EQ(msgs[0].chat_name, "chat1");
    EXPECT_EQ(msgs[0].text, "message1 1");
    EXPECT_EQ(msgs[0].attachment, "/image/1_1");
    EXPECT_EQ(msgs[1].id, 2);
    EXPECT_EQ(msgs[1].sender_id, 2);
    EXPECT_EQ(msgs[1].chat_id, chatIDTest);
    EXPECT_TRUE(msgs[1].text.empty());
    EXPECT_EQ(msgs[1].attachment, "/image/2_1");
}

TEST(DBLibTest, GetChatMessagesAfterID) {
    PostgresDB postgres(CONFIG);
    unsigned long chatIDTest = 3;
    std::vector<Message> msgs;
    EXPECT_NO_THROW(msgs = postgres.GetChatMessagesAfterID(chatIDTest, 5));
    EXPECT_EQ(msgs[0].id, 6);
    EXPECT_EQ(msgs[0].sender_id, 4);
    EXPECT_EQ(msgs[0].sender_name, "name4");
    EXPECT_EQ(msgs[0].chat_id, chatIDTest);
    EXPECT_EQ(msgs[0].chat_name, "chat3");
    EXPECT_EQ(msgs[0].text, "message3 3");
    EXPECT_EQ(msgs[0].attachment, "/image/2_3");
    EXPECT_NO_THROW(msgs = postgres.GetChatMessagesAfterID(chatIDTest, 6));
    EXPECT_EQ(msgs.size(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
