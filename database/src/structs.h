#pragma once

#include <string>
#include <vector>

struct User {
    int id;
    std::string NickName;
    std::string ProfileAvatar;
    std::string CreatedAt;
    std::vector<int> ChatsID;
};

struct UserForm {
    std::string NickName;
    std::string ProfileAvatar;
};

struct Chat {
    int id;
    std::string ChatName;
    std::string CreatedAt;
    int TotalMessages;
    std::vector<int> UsersID;
};

struct ChatForm {
    std::string ChatName;
    std::vector<int> UsersID;
};

struct Message {
    int id;
    int SenderID;
    int ChatID;
    std::string Message;
    std::vector<std::string> Attachment;
    std::string CreatedAt;
    int NumInChat;
};

struct MessageForm {
    int SenderID;
    int ChatID;
    std::string Message;
    std::vector<std::string> Attachment;
};
