#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger
{
public:
    static void create();
    static void close();
private:
    static std::string getFileName();

    static std::ofstream logFile;
    static const std::string headers[];
};

#endif