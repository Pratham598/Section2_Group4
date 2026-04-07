#include "Client.h"
#include "Server.h"
#include "Logger.h"
#include <iostream>

Client::Client(const std::string& username, const std::string& password, Server& server) 
    : username(username), password(password), server(server) {
    Logger::log("Client " + username + " created.");
}

Client::~Client() {}

void Client::login() {
    Logger::log("Client " + username + " attempting to login.");
    if (server.authenticateUser(username, password)) {
        std::cout << "[Client] Login successful for user: " << username << ".\n";
    } else {
        std::cout << "[Client] Login failed.\n";
    }
}

void Client::selectCamera() {
    int cameraId;
    std::cout << "Enter Camera ID to select: ";
    std::cin >> cameraId;
    std::cout << "[Client] Camera " << cameraId << " selected.\n";
    
    // Convert choice to string and test sending request
    sendRequest("SELECT_CAMERA_" + std::to_string(cameraId));
}

void Client::sendRequest(const std::string& requestStr) {
    Logger::log("Client " + username + " sending request: " + requestStr);
    
    // Create Packet object to send requests
    Packet packet(1, 1, requestStr); 
    
    // Simulate communication with Server class
    server.processRequest(packet);
}

void Client::receiveResponse(const std::string& response) {
    Logger::log("Client " + username + " received response: " + response);
    std::cout << "[Client] Server Response: " << response << "\n";
}

void Client::menu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== Traffic Monitoring Client Menu ===\n"
                  << "1. Login\n"
                  << "2. Start Monitoring\n"
                  << "3. Stop Monitoring\n"
                  << "4. Get Snapshot\n"
                  << "5. Select Camera\n"
                  << "0. Exit\n"
                  << "Enter choice: ";
                  
        if (!(std::cin >> choice)) {
            // Check to avoid infinite loop from bad input
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                sendRequest("START_MONITOR");
                break;
            case 3:
                sendRequest("STOP_MONITOR");
                break;
            case 4:
                sendRequest("GET_SNAPSHOT");
                break;
            case 5:
                selectCamera();
                break;
            case 0:
                std::cout << "Exiting menu...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
