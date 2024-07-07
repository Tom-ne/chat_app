#pragma once
#include "IDatabase.h"
#include "sqlite3.h"

class SqliteDatabase : public IDatabase
{
public:
    bool open() override;
    void close() override;

    bool signup(const std::string& username, const std::string& password) override;
    bool login(const std::string& username, const std::string& password) override;

protected:
    bool doesUserExist(const std::string& username) override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) override;
private:

    sqlite3* db;
};
