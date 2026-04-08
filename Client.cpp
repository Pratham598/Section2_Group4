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
    
    // Send request to Server and receive response Packets
    std::vector<Packet> responsePackets = server.processRequest(requestPacket);
    
    std::string assembledSnapshot = "";
    
    for (const auto& responsePacket : responsePackets) {
        std::string payload = responsePacket.payload;
        
        if (payload.length() >= 6 && payload.substr(0, 6) == "CHUNK:") {
            // Parse chunk header. Format: CHUNK:<index>/<total>:<data>
            size_t headerEnd = payload.find(':', 6);
            if (headerEnd != std::string::npos) {
                assembledSnapshot += payload.substr(headerEnd + 1);
            }
        } else {
            receiveResponse(responsePacket);
        }
    }
    
    if (!assembledSnapshot.empty()) {
        // Create a summary packet to indicate successful assembly without overwhelming the console
        std::string summary = "Snapshot fully assembled (" + std::to_string(assembledSnapshot.size()) + " simulated bytes).";
        Packet summaryPacket(2, requestPacket.sequenceNumber, summary);
        receiveResponse(summaryPacket);
    }
}

void Client::receiveResponse(const Packet& response) {
    std::string logPayload = response.payload;
    if (logPayload.length() > 60) {
        logPayload = logPayload.substr(0, 60) + "... [TRUNCATED]";
    }
    Logger::log("Client " + username + " received response: " + logPayload);
    std::cout << "    >> Server: " << logPayload << "\n";
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
            // Check for End-Of-File (e.g. Ctrl+D) to prevent infinite loops
            if (std::cin.eof()) {
                std::cout << "\n[i] Input stream closed. Shutting down Client System. Goodbye!\n";
                break;
            }
            
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
