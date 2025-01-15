#include "../include/Logger.hpp"
#include "iostream"

// Конструктор: открывает файл журнала и устанавливает уровень важности
Logger::Logger(const std::string& filename, LogLevel defaultLevel) : currentLevel(defaultLevel) {
    logFile.open(filename, std::ios::app);  
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file:" + filename);
    }
}

// Деструктор: закрывает файл журнала
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Метод для записи сообщения в журнал
void Logger::log(const std::string& message, LogLevel level) {
    if (level < currentLevel) {
        return;        // Не записываем сообщения с уровнем ниже текущего
    }

    std::lock_guard<std::mutex> lock(logMutex);        // Захватываем мьютекс для потокобезопасности

    // Записываем сообщение в формате: [время] [уровень] сообщение
    logFile << "[" << getCurrentTime() << "]" << "[" << levelToString(level) << "]" << message << std::endl;
}

// Вспомогательный метод для получения текущего времени
std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H-%M-%S", &tm);
    return std::string(buffer);
}

// Вспомогательный метод для преобразования уровня важности в строку
std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

// Метод для изменения уровня важности по умолчанию
void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}
