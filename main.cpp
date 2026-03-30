#include <iostream>
#include "Server.h"
#include "Client.h"

int main() {
    std::cout << "--- Traffic Camera Monitoring System ---" << std::endl;

    Server monitoringServer;
    monitoringServer.start();

    Client camera1(101, monitoringServer);
    Client camera2(102, monitoringServer);

    camera1.sendData("Speed violation detected: 85 mph");
    camera2.sendData("Red light violation");

    monitoringServer.stop();

    return 0;
}
