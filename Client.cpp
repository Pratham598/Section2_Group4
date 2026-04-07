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
        std::cout << "[+] SUCCESS: Login successful for user '" << username << "'.\n";
    } else {
        std::cout << "[-] ERROR: Login failed for user '" << username << "'.\n";
    }
}

void Client::sendRequest(const std::string& requestStr) {
    Logger::log("Client " + username + " sending request: " + requestStr);
    
    // Create Packet object to send requests (type 1 for client request)
    static int sequenceNumber = 1;
    Packet requestPacket(1, sequenceNumber++, requestStr); 
    
    // Send request to Server and receive response Packet
    Packet responsePacket = server.processRequest(requestPacket);
    receiveResponse(responsePacket);
}

void Client::receiveResponse(const Packet& response) {
    Logger::log("Client " + username + " received response: " + response.payload);
    std::cout << "    >> Server: " << response.payload << "\n";
}

void Client::menu() {
    int choice = -1;
    while (choice != 5) {
        std::cout << "\n========================================\n"
                  << "    Traffic Monitoring Client System    \n"
                  << "========================================\n"
                  << "  1. Login\n"
                  << "  2. Start Monitoring\n"
                  << "  3. Stop Monitoring\n"
                  << "  4. Get Snapshot\n"
                  << "  5. Exit\n"
                  << "========================================\n"
                  << "Select an option (1-5): ";
                  
        if (!(std::cin >> choice)) {
            // Buffer clear for invalid types (e.g. letters)
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n[!] ERROR: Invalid input. Please enter a number from 1 to 5.\n";
            continue;
        }
        
        // Ignore any remaining trailing characters
        std::cin.ignore(10000, '\n');
        
        std::cout << "\n"; // Clean spacing before action executes
        
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
                std::cout << "[i] Shutting down Client System. Goodbye!\n";
                break;
            default:
                std::cout << "[!] ERROR: Option out of range. Please choose 1 through 5.\n";
        }
    }
}
