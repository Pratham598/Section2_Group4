#include "Server.h"
#include "Logger.h"

Server::Server() : currentState(ServerState::IDLE), isAuthenticated(false) {
    Logger::log("Server initialized in IDLE state.");
}

Server::~Server() {}

bool Server::authenticateUser(const std::string& username, const std::string& password) {
    Logger::log("Authenticating user: " + username);
    if (username == "admin" && password == "password123") {
        isAuthenticated = true;
        Logger::log("Authentication successful.");
        return true;
    }
    Logger::log("Authentication failed.");
    return false;
}

Packet Server::processRequest(const Packet& p) {
    Logger::logPacket("REQ", p);
    
    if (!isAuthenticated) {
        Logger::log("Unauthorized request. Please authenticate first.");
        Packet response(2, p.sequenceNumber, "Unauthorized request.");
        Logger::logPacket("RES", response);
        return response;
    }

    std::string request = p.payload;
    Logger::log("Processing request: " + request);
    std::string responseStr;

    // State Machine Implementation
    switch (currentState) {
        case ServerState::IDLE:
            if (request == "START_MONITOR") {
                currentState = ServerState::MONITORING;
                responseStr = "Server state changed to MONITORING.";
            } else if (request == "STOP_MONITOR" || request == "GET_SNAPSHOT") {
                responseStr = "Error: Rejected invalid action '" + request + "' in IDLE state.";
            } else {
                responseStr = "Error: Unknown request '" + request + "'.";
            }
            break;

        case ServerState::MONITORING:
            if (request == "STOP_MONITOR") {
                currentState = ServerState::IDLE;
                responseStr = "Server state changed to IDLE.";
            } else if (request == "START_MONITOR") {
                responseStr = "Error: Rejected invalid action. Server is already MONITORING.";
            } else if (request == "GET_SNAPSHOT") {
                responseStr = "Snapshot transfer complete.";
            } else if (request.length() >= 7 && request.substr(0, 7) == "CAMERA_") {
                responseStr = request + "_STREAM_START";
            } else {
                responseStr = "Error: Unknown request '" + request + "'.";
            }
            break;
    }

    manageState(); // Prints current state for debugging
    Logger::log("[RESPONSE] " + responseStr);
    
    // Return a response packet (type 2 to denote server response)
    Packet response(2, p.sequenceNumber, responseStr);
    Logger::logPacket("RES", response);
    return response;
}

void Server::manageState() {
    switch (currentState) {
        case ServerState::IDLE:
            Logger::log("[STATE] Server is currently IDLE. Waiting for commands.");
            break;
        case ServerState::MONITORING:
            Logger::log("[STATE] Server is currently MONITORING traffic cameras.");
            break;
    }
}
