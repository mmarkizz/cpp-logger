#include "logger.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

Logger::Logger(const std::string& filename, LogLevel level)
    : currentLevel(level)
{
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message, LogLevel level) {
    if (level < currentLevel) return;

    std::lock_guard<std::mutex> lock(logMutex);
    logFile << "[" << getTimestamp() << "]"
        << " [" << levelToString(level) << "] "
        << message << std::endl;
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

std::string Logger::getTimestamp() const {
    auto now = std::time(nullptr);
    std::tm tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::INFO: return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}
