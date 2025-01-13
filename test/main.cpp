#include "../include/Logger.hpp"
#include <thread>

void testLogger() {
    Logger logger("test.log", LogLevel::DEBUG);

    logger.log("This is a debug message", LogLevel::DEBUG);
    logger.log("This is an info message", LogLevel::INFO);
    logger.log("This is a warning", LogLevel::WARNING);
    logger.log("This is an error", LogLevel::ERROR);

    logger.setLogLevel(LogLevel::WARNING);
    logger.log("This message should not be logged", LogLevel::INFO);
}

int main() {
    testLogger();
    return 0;
}