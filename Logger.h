#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

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
};

#endif // LOGGER_H
