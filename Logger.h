#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include "Packet.h"

class Logger {
private:
    std::vector<std::string> logEntries;
    std::string filename;

    std::string getCurrentTimestamp();

public:
    Logger(const std::string& logFilename = "log.txt");
    

    void logMessage(std::string msg);
    
    // Writes all buffered logs to the text file
    void writeToFile();
    
    // Static log method for components to use
    static void log(const std::string& msg);
    
    // Logs a specific packet transaction with direction (REQ/RES)
    static void logPacket(const std::string& direction, const Packet& p);
};

#endif // LOGGER_H
