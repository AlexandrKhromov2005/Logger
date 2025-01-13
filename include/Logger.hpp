#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string& filename, LogLevel defaultLevel = LogLevel::INFO);

    ~Logger();

    void log(const std::string& message, LogLevel level = LogLevel::INFO);

    void setLogLevel(LogLevel level);

private:
    std::ofstream logFile;
    LogLevel currentLevel;
    std::mutex logMutex;

    std::string getCurrentTime() const;

    std::string levelToString(LogLevel level) const;
};

#endif // LOGGER_HPP