#include "logger.h"
#include <iostream>

int main() {
    try {
        Logger logger("log.txt", LogLevel::WARNING);

        std::cout << "Logging INFO (должно быть проигнорировано):" << std::endl;
        logger.log("This is an info message", LogLevel::INFO);

        std::cout << "Logging WARNING (должно попасть в лог):" << std::endl;
        logger.log("This is a warning message", LogLevel::WARNING);

        std::cout << "Logging ERROR (должно попасть в лог):" << std::endl;
        logger.log("This is an error message", LogLevel::ERROR);

        std::cout << "Changing default log level to INFO..." << std::endl;
        logger.setLogLevel(LogLevel::INFO);
        logger.log("Now even info messages will be logged", LogLevel::INFO);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
