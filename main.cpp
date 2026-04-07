#include <iostream>
#include "Server.h"
#include "Client.h"

int main() {
    std::cout << "--- Traffic Camera Monitoring System : Control Setup ---" << std::endl;

    Server monitoringServer;
    monitoringServer.runLoop(); // Shows initial IDLE state

    Client camera1("admin", "password123", monitoringServer);
    
    // 1. Unauthenticated request
    camera1.sendRequest("START_MONITOR");

    // 2. Authenticate using client's new login method
    camera1.login();

    // 3. Authenticated requests
    camera1.sendRequest("GET_SNAPSHOT");   // Fails because server is IDLE
    camera1.sendRequest("START_MONITOR");  // Changes state to MONITORING
    camera1.sendRequest("START_MONITOR");  // Duplicate check
    
    // 4. Simulate getting a snapshot
    camera1.sendRequest("GET_SNAPSHOT");

    // 5. Stop monitoring
    camera1.sendRequest("STOP_MONITOR");

    // 6. Provide the interactive menu requested in instructions
    // camera1.menu(); // Uncomment to use the interactive menu

    return 0;
}
