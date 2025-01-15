#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>

// Уровни важности сообщений
enum class LogLevel {
    DEBUG,    // Отладочные сообщения
    INFO,    // Информационные сообщения
    WARNING,    // Предупреждения
    ERROR    // Ошибки
};

// Класс Logger для записи сообщений в журнал
class Logger {
public:
    // Конструктор: принимает имя файла журнала и уровень важности по умолчанию
    Logger(const std::string& filename, LogLevel defaultLevel = LogLevel::INFO);

    // Деструктор: закрывает файл журнала
    ~Logger();

    // Метод для записи сообщения в журнал
    void log(const std::string& message, LogLevel level = LogLevel::INFO);

    // Метод для изменения уровня важности по умолчанию
    void setLogLevel(LogLevel level);

private:
    std::ofstream logFile;    // Файл для записи журнала
    LogLevel currentLevel;    // Текущий уровень важности
    std::mutex logMutex;    // Мьютекс для потокобезопасности

    // Вспомогательный метод для получения текущего времени
    std::string getCurrentTime() const;

    // Вспомогательный метод для преобразования уровня важности в строку
    std::string levelToString(LogLevel level) const;
};

#endif // LOGGER_HPP
