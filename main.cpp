#include <iostream>
#include "Server.h"
#include "Client.h"
#include "Packet.h"
#include "Logger.h"

int main() {
    std::cout << "--- Traffic Camera Monitoring System : Integration Setup ---" << std::endl;

    // Create objects of all classes
    Logger mainLogger("requests_log.txt");
    Server monitoringServer;
    
    mainLogger.logMessage("Server initialized.");
    
    Client client("admin", "password123", monitoringServer);
    
    // Authenticate the user so server accepts requests
    client.login();
    
    // Request 1: START_MONITOR
    mainLogger.logMessage("Simulating explicit flow for START_MONITOR");
    Packet pktStart(1, 1, "START_MONITOR");
    monitoringServer.processRequest(pktStart);
    client.receiveResponse("Server state changed to MONITORING.");
    
    // Request 2: GET_SNAPSHOT
    mainLogger.logMessage("Simulating explicit flow for GET_SNAPSHOT");
    Packet pktSnap(1, 2, "GET_SNAPSHOT");
    monitoringServer.processRequest(pktSnap);
    client.receiveResponse("Snapshot transfer complete.");

    // Request 3: STOP_MONITOR
    mainLogger.logMessage("Simulating explicit flow for STOP_MONITOR");
    Packet pktStop(1, 3, "STOP_MONITOR");
    monitoringServer.processRequest(pktStop);
    client.receiveResponse("Server state changed to IDLE.");

    // Write stored logs
    mainLogger.writeToFile();
    
    std::cout << "--- Integration test completed successfully. ---" << std::endl;
    return 0;
}
