#include <iostream>
#include "Server.h"
#include "Client.h"
#include "Logger.h"

int main() {
    std::cout << "--- Traffic Camera Monitoring System : Integration Setup ---\n" << std::endl;

    // Create objects of all classes
    Logger mainLogger("requests_log.txt");
    Server monitoringServer;
    
    mainLogger.logMessage("Server initialized.");
    
    Client client("admin", "password123", monitoringServer);
    
    // Authenticate the user so server accepts requests
    client.login();
    
    // Request 1: START_MONITOR
    mainLogger.logMessage("Simulating explicit flow for START_MONITOR");
    client.sendRequest("START_MONITOR");
    
    // Request 2: GET_SNAPSHOT
    mainLogger.logMessage("Simulating explicit flow for GET_SNAPSHOT");
    client.sendRequest("GET_SNAPSHOT");

    // Request 3: STOP_MONITOR
    mainLogger.logMessage("Simulating explicit flow for STOP_MONITOR");
    client.sendRequest("STOP_MONITOR");

    // Request 4: CAMERA_1 (testing the new logic)
    mainLogger.logMessage("Simulating explicit flow for CAMERA_1");
    client.sendRequest("CAMERA_1");

    // Write stored logs
    mainLogger.writeToFile();
    
    std::cout << "--- Integration test completed successfully. ---\n" << std::endl;
    return 0;
}
