#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

class Logger {
public:
    static void initialize();
    static void finalize();
    static void log(const std::string& data);
private:
    static const std::string getLogFileName();
    static const int getCurrentDateTime();

    static std::ofstream logFile;
    static const std::string headers[];
};

#endif // LOGGER_H