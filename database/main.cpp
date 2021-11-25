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
        user = db.AddUser(userForm);
        cout << user.id << " " << user.nickname << " " << user.profile_avatar
             << " " << user.created_at << endl;
    }
    catch (std::exception &ex) {
        std::cerr << "Something go wrong2: " << ex.what() << std::endl;
    }

    return 0;
}

