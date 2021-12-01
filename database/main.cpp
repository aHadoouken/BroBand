#include <iostream>
#include "db_implementation.h"

using namespace std;

int main() {

    try {
        PostgresDB db("/home/alex/mail_cpp/BroBand/database/db_config.cfg");
        UserForm userForm;
        User user;
        userForm.nickname = "alex113";
        userForm.profile_avatar = "/user/aaa2a.png";
        user = db.ExtractUserByID(1);
        cout << user.id << " " << user.nickname << " " << user.profile_avatar
             << " " << user.created_at << endl;
        for (auto &el:user.chats_id){
            std::cout << el << " ";
        }
        std::cout << std::endl;

        user = db.ExtractUserByNickName("user2");
        cout << user.id << " " << user.nickname << " " << user.profile_avatar
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
    }
    catch (pqxx::plpgsql_no_data_found &ex) {
        std::cerr << "Something go wrong1: " << ex.what() << std::endl;
    }
    catch (std::exception &ex) {
        std::cerr << "Something go wrong2: " << ex.what() << std::endl;
    }

    return 0;
}

