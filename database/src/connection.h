#pragma once
#include <pqxx/pqxx>

class Connection{
public:
    static Connection &getInstance() {
        static Connection _instance;
        return _instance;
    }

private:
    Connection(){}
private:
    pqxx::connection connect;
};
