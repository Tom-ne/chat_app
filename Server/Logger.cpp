#include "Logger.h"
#include <stdexcept>
#include <iostream>
#include <chrono>

std::ofstream Logger::logFile;
const std::string Logger::headers[] = { "time", "action" };

void Logger::initialize() {
    // open the file
    std::string filename = getLogFileName();
    logFile.open(filename);

    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // write headers to file
    for (size_t i = 0; i < sizeof(headers) / sizeof(headers[0]); ++i) {
        logFile << headers[i];
        if (i != sizeof(headers) / sizeof(headers[0]) - 1) {
            logFile << ",";
        }
    }
    logFile << "\n";
}

void Logger::finalize() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& data) {
    logFile << getCurrentDateTime() << "," << data << "\n";
}

const std::string Logger::getLogFileName()
{
    return "log-" + std::to_string(getCurrentDateTime()) + ".csv";
}

const int Logger::getCurrentDateTime() {
    uint64_t sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return sec;
}
