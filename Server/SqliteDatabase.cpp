#include "SqliteDatabase.h"
#include "Logger.h"
#include <io.h>

const std::string DATABASE_FILE_NAME = "database.sqlite";

bool SqliteDatabase::open()
{
    int doesDatabaseExist = _access(DATABASE_FILE_NAME.c_str(), 0);
    int res = sqlite3_open(DATABASE_FILE_NAME.c_str(), &this->db);
    if (doesDatabaseExist != 0) {
        const std::string dbInitialize =
            "CREATE TABLE Users (username VARCHAR(50) PRIMARY KEY, password VARCHAR(255) NOT NULL);"
            "CREATE TABLE Friends (user_id VARCHAR(50), friends JSONB, PRIMARY KEY (user_id), FOREIGN KEY (user_id) REFERENCES Users(username));"
            "CREATE TABLE Chats (chat_id INTEGER PRIMARY KEY AUTOINCREMENT, chat_name VARCHAR(100), users TEXT, messages TEXT);";

        char* error = NULL;
        res = sqlite3_exec(this->db, dbInitialize.c_str(), nullptr, nullptr, &error);
    }

    if (res != SQLITE_OK) {
        this->db = nullptr;
        Logger::log("Failed to open database");
        return false;
    }
    return true;
}

void SqliteDatabase::close()
{
    if (this->db) {
        sqlite3_close(this->db);
        this->db = nullptr;
    }
}

bool SqliteDatabase::doesUserExist(const std::string& username)
{
    std::string queryString = "SELECT COUNT(*) FROM Users WHERE username = ?";
    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    // prepare the SQL statement
    int res = sqlite3_prepare_v2(db, queryString.c_str(), -1, &stmt, nullptr);
    if (res != SQLITE_OK)
    {
        Logger::log("Error preparing SQL statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    // bind the username parameter
    res = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    if (res != SQLITE_OK)
    {
        Logger::log("Error binding parameter: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    // execute the SQL statement
    res = sqlite3_step(stmt);
    if (res == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }
    else if (res != SQLITE_DONE)
    {
        Logger::log("Error executing SQL statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    // finalize the statement and close
    sqlite3_finalize(stmt);

    return (count > 0);
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
    std::string queryString = "SELECT password FROM Users WHERE username = ?";
    std::string storedPassword;

    sqlite3_stmt* stmt = nullptr; // Define stmt here

    // Prepare the SQL statement
    int res = sqlite3_prepare_v2(db, queryString.c_str(), -1, &stmt, nullptr);
    if (res != SQLITE_OK)
    {
        Logger::log("Error preparing SQL statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    // Bind parameters using the utility function
    res = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    if (res != SQLITE_OK)
    {
        Logger::log("Error binding parameter: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    // Execute the SQL statement
    res = sqlite3_step(stmt);
    if (res == SQLITE_ROW)
    {
        storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    else if (res != SQLITE_DONE)
    {
        Logger::log("Error executing SQL statement: " + std::string(sqlite3_errmsg(db)));
        sqlite3_finalize(stmt);
        return false;
    }

    // Finalize the statement and close
    sqlite3_finalize(stmt);

    // Compare stored password with provided password
    return (storedPassword == password);
}