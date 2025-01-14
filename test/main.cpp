#include "../include/Logger.hpp"
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

std::queue<std::pair<std::string, LogLevel>> messageQueue;
std::mutex queueMutex;
std::condition_variable queueCondition;
std::atomic<bool> stopThread(false);

void logWorker(Logger& logger){
    while (true) {
        std::pair<std::string, LogLevel> message;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, []{return !messageQueue.empty() || stopThread;});

            if (stopThread && messageQueue.empty()) {
                break;
            }

            message = messageQueue.front();
            messageQueue.pop();
        }

        logger.log(message.first, message.second);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <journal name> <level of log>" << std::endl;
    }

    std::string logFile = argv[1];
    LogLevel defaultLevel = static_cast<LogLevel>(std::stoi(argv[2]));
    Logger logger(logFile, defaultLevel);
    std::thread worker(logWorker, std::ref(logger));
    std::string input;

    while (true) {
        std::cout << "Enter your message (or 'exit' to stop)";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        std::cout << "Enter log level (0 - DEBUG, 1 - INFO, 2 - WARNING, 3 - ERROR): ";
        std::string levelInput;
        std::getline(std::cin, levelInput);

        LogLevel level = defaultLevel;
        if(!levelInput.empty()) {
            level = static_cast<LogLevel>(std::stoi(levelInput));
        }

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            messageQueue.push({input, level});
        }

        queueCondition.notify_one();
    }

    stopThread = true;
    queueCondition.notify_all();
    worker.join();

    std::cout << "Programm stopped." << std::endl;

    return 0;
}
