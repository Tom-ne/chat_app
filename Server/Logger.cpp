#include "Logger.h"
#include <stdexcept>

std::ofstream Logger::logFile;

const std::string Logger::headers[] = { "time", "action" };

std::string Logger::getFileName()
{
    return "logFile.csv"; // TODO: Implement filename logic here
}

void Logger::create()
{
    std::string filename = getFileName();

    logFile.open(filename);

    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    for (size_t i = 0; i < sizeof(headers) / sizeof(headers[0]); ++i) {
        logFile << headers[i];
        if (i != sizeof(headers) / sizeof(headers[0]) - 1) {
            logFile << ",";
        }
    }
    logFile << "\n";
}

void Logger::close()
{
    logFile.close();
}
