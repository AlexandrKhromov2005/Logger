#include "../include/Logger.hpp"
#include "iostream"

Logger::Logger(const std::string& filename, LogLevel defaultLevel) : currentLevel(defaultLevel) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()){
            throw std::runtime_error("Failed to open log file:" + filename);
        }
}

Logger::~Logger(){
    if (logFile.is_open()){
        logFile.close();
    }
}

void Logger::log(const std::string& message, LogLevel level) {
    if (level < currentLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(logMutex);

    logFile << "[" << getCurrentTime() << "]" << "[" << levelToString(level) << "]" << message << std::endl;
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H-%M-%S", &tm);
    return std::string(buffer);
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG :
            return "DEBUG";
        case LogLevel::INFO :
            return "INFO";
        case LogLevel::WARNING :
            return "WARNING";
        case LogLevel::ERROR :
            return "ERROR";
        default :
            return "UNKNOWN";
    }
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}