#pragma once
#include <string>

class IDatabase
{
public:
	virtual bool open() = 0;
	virtual void close() = 0;

	virtual bool signup(const std::string& username, const std::string& password) = 0;
	virtual bool login(const std::string& usernmae, const std::string& password) = 0;
protected:
	virtual bool doesUserExist(const std::string& username) = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;

};

