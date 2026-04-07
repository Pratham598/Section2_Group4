#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

Logger::Logger(const std::string& logFilename) : filename(logFilename) {
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&now_time);
    
    std::stringstream ss;
    if (local_tm) {
        ss << std::put_time(local_tm, "%Y-%m-%d %H:%M:%S");
    }
    return ss.str();
}

void Logger::logMessage(std::string msg) {
    // Combine the timestamp with the message (which can contain packet type and payload size)
    std::string logEntry = "[" + getCurrentTimestamp() + "] " + msg;
    
    // Store in memory
    logEntries.push_back(logEntry);
    
    // Also output to console for immediate visibility
    std::cout << logEntry << std::endl; 
}

void Logger::writeToFile() {
    if (logEntries.empty()) {
        return;
    }

    // Open file in append mode
    std::ofstream outFile(filename, std::ios::app);
    if (outFile.is_open()) {
        for (const auto& entry : logEntries) {
            outFile << entry << "\n";
        }
        outFile.close();
        
        // Clear the buffer after writing
        logEntries.clear();
    } else {
        std::cerr << "Error: Unable to open log file (" << filename << ") for writing." << std::endl;
    }
}

void Logger::log(const std::string& msg) {
    // Create a temporary logger to print out the static logs instead of losing them
    static Logger sysLogger("system_log.txt");
    sysLogger.logMessage(msg);
    sysLogger.writeToFile();
}
