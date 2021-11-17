#include <exception>
#include <iostream>
#include "Server.h"

int main() {
    Server s;
    try {
        s.Run();
    }
    catch (std::exception &ex) {
        std::cerr << "Something go wrong: " << ex.what() << std::endl;
    }
}

