#include "logger.h"
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <sstream>
#include <map>

struct LogTask {
    std::string message;
    LogLevel level;
};

std::queue<LogTask> logQueue;
std::mutex queueMutex;
std::condition_variable queueCond;
std::atomic<bool> running{ true };

LogLevel parseLogLevel(const std::string& levelStr, LogLevel defaultLevel) {
    static std::map<std::string, LogLevel> levelMap = {
        {"INFO", LogLevel::INFO},
        {"WARNING", LogLevel::WARNING},
        {"ERROR", LogLevel::ERROR}
    };

    auto it = levelMap.find(levelStr);
    if (it != levelMap.end()) return it->second;
    return defaultLevel;
}

void logWorker(Logger& logger) {
    while (running || !logQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCond.wait(lock, [] { return !logQueue.empty() || !running; });

        while (!logQueue.empty()) {
            LogTask task = logQueue.front();
            logQueue.pop();
            lock.unlock();
            logger.log(task.message, task.level);
            lock.lock();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./logconsole <logfile> <default_level>" << std::endl;
        return 1;
    }

    std::string logFile = argv[1];
    LogLevel defaultLevel = parseLogLevel(argv[2], LogLevel::INFO);

    try {
        Logger logger(logFile, defaultLevel);

        std::thread worker(logWorker, std::ref(logger));

        std::cout << "Enter messages (type 'exit' to quit):" << std::endl;
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "exit") break;

            std::istringstream iss(line);
            std::string word;
            std::string msg;
            std::string levelStr;
            while (iss >> word) {
                if (word == "INFO" || word == "WARNING" || word == "ERROR") {
                    levelStr = word;
                    break;
                }
                msg += word + " ";
            }

            LogLevel level = parseLogLevel(levelStr, defaultLevel);

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                logQueue.push({ msg, level });
            }
            queueCond.notify_one();
        }

        running = false;
        queueCond.notify_all();
        worker.join();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
