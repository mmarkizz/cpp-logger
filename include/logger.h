#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    Logger(const std::string& filename, LogLevel level);
    ~Logger();

    void log(const std::string& message, LogLevel level);
    void setLogLevel(LogLevel level);

private:
    std::ofstream logFile;
    LogLevel currentLevel;
    std::mutex logMutex;

    std::string getTimestamp() const;
    std::string levelToString(LogLevel level) const;
};

#endif // LOGGER_H
