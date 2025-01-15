#include "../include/Logger.hpp"
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

std::queue<std::pair<std::string, LogLevel>> messageQueue;    // Очередь для хранения сообщений
std::mutex queueMutex;    // Мьютекс для защиты очереди
std::condition_variable queueCondition;    // Условная переменная для уведомлений
std::atomic<bool> stopThread(false);    // Флаг для завершения потока

// Функция для потока, который записывает сообщения в журнал
void logWorker(Logger& logger) {
    while (true) {
        std::pair<std::string, LogLevel> message;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Ожидаем уведомления, если очередь пуста и поток не завершен
            queueCondition.wait(lock, [] { return !messageQueue.empty() || stopThread; });

            // Завершаем поток, если очередь пуста и флаг остановки установлен
            if (stopThread && messageQueue.empty()) {
                break;
            }

            // Получаем сообщение из очереди
            message = messageQueue.front();
            messageQueue.pop();
        }

        // Записываем сообщение в журнал
        logger.log(message.first, message.second);
    }
}

int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <journal name> <level of log>" << std::endl;
        return 1;
    }

    std::string logFile = argv[1];  // Имя файла журнала
    LogLevel defaultLevel = static_cast<LogLevel>(std::stoi(argv[2]));  // Уровень важности по умолчанию

    Logger logger(logFile, defaultLevel);  // Инициализация логгера
    std::thread worker(logWorker, std::ref(logger));  // Запуск потока для записи сообщений

    std::string input;
    while (true) {
        std::cout << "Enter your message (or 'exit' to stop): ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        std::cout << "Enter log level (0 - DEBUG, 1 - INFO, 2 - WARNING, 3 - ERROR): ";
        std::string levelInput;
        std::getline(std::cin, levelInput);

        LogLevel level = defaultLevel;
        if (!levelInput.empty()) {
            level = static_cast<LogLevel>(std::stoi(levelInput));  // Преобразуем ввод в уровень важности
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);  // Захватываем мьютекс
            messageQueue.push({input, level});  // Добавляем сообщение в очередь
        }
        queueCondition.notify_one();  // Уведомляем поток о новом сообщении
    }

    // Завершение работы потока
    stopThread = true;
    queueCondition.notify_all();  // Уведомляем поток о завершении
    worker.join();  // Ожидаем завершения потока

    std::cout << "Program stopped." << std::endl;

    return 0;
}
