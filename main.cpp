#include "Server.h"
#include "Packet.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <exception>

int main() {
  // VERY FIRST LINE DEBUG PRINT
  std::cout << "SERVER STARTED" << std::endl;
  
  // Sleep before entering the main logic or initializing objects to confirm execution
  std::this_thread::sleep_for(std::chrono::seconds(2));

  try {
      Server server;
      server.authenticateUser("admin", "password123");

      std::vector<std::string> requests = {"START_MONITOR", "GET_SNAPSHOT", "STOP_MONITOR"};
      int index = 0;
      int seq = 1;

      while (true) {
        std::cout << "[SERVER] Waiting for requests..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::string currentRequest = requests[index % requests.size()];
        std::cout << "[SERVER] Received request: " << currentRequest << std::endl;

        Packet req(1, seq++, currentRequest);
        server.processRequest(req);

        if (currentRequest == "START_MONITOR") {
          std::cout << "[SERVER] State changed to MONITORING" << std::endl;
        } else if (currentRequest == "GET_SNAPSHOT") {
          std::cout << "[SERVER] Snapshot request processed." << std::endl;
        } else if (currentRequest == "STOP_MONITOR") {
          std::cout << "[SERVER] State changed to IDLE" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
        index++;
      }
  } catch (const std::exception& e) {
      std::cout << "[FATAL ERROR] " << e.what() << std::endl;
  } catch (...) {
      std::cout << "[FATAL ERROR] Unknown crash!" << std::endl;
  }

  return 0;
}