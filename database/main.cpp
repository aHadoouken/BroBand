#include <iostream>
#include "db_implementation.h"

using namespace std;

int main() {

    try {
        PostgresDB db("/home/alex/mail_cpp/BroBand/database/db_config.cfg");
        UserForm userForm;
        User user;
        userForm.login = "alex113";
        userForm.profile_avatar = "/user/aaa2a.png";
        user = db.GetUserByID(1);
        cout << user.id << " " << user.login << " " << user.profile_avatar
             << " " << user.created_at << endl;
        for (auto &el:user.chats_id){
            std::cout << el << " ";
        }
        std::cout << std::endl;

        user = db.GetUserByLogin("user2");
        cout << user.id << " " << user.login << " " << user.profile_avatar
             << " " << user.created_at << endl;
        for (auto &el:user.chats_id){
            std::cout << el << " ";
        }
        std::cout << std::endl;

        ChatForm chatForm;
        Chat chat;
        std::vector<unsigned long> existingUsersID = {1, 2, 3};
        chatForm.chat_name = "chatname";
        chatForm.users_id = existingUsersID;
        chat = db.AddChat(chatForm);


        MessageForm msgForm;
        Message msg;
        msgForm.sender_id = 1;
        msgForm.chat_id = 1;
        msgForm.text = "msg";
        msg = db.AddMessage(msgForm);
    }
    catch (pqxx::plpgsql_no_data_found &ex) {
        std::cerr << "Something go wrong1: " << ex.what() << std::endl;
    }
    catch (std::exception &ex) {
        std::cerr << "Something go wrong2: " << ex.what() << std::endl;
    }

    return 0;
}

