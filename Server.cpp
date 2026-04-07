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
    if (!isAuthenticated) {
        Logger::log("Unauthorized request. Please authenticate first.");
        return Packet(2, p.sequenceNumber, "Unauthorized request.");
    }

    std::string request = p.payload;
    Logger::log("Processing request: " + request);
    std::string responseStr;

    if (request == "START_MONITOR") {
        if (currentState == ServerState::IDLE) {
            currentState = ServerState::MONITORING;
            responseStr = "Server state changed to MONITORING.";
        } else {
            responseStr = "Server is already monitoring.";
        }
    } else if (request == "STOP_MONITOR") {
        if (currentState == ServerState::MONITORING) {
            currentState = ServerState::IDLE;
            responseStr = "Server state changed to IDLE.";
        } else {
            responseStr = "Server is already idle.";
        }
    } else if (request == "GET_SNAPSHOT") {
        if (currentState == ServerState::MONITORING) {
            responseStr = "Snapshot transfer complete.";
        } else {
            responseStr = "Cannot get snapshot. Server is not in MONITORING state.";
        }
    } else if (request.length() >= 7 && request.substr(0, 7) == "CAMERA_") {
        currentState = ServerState::MONITORING;
        responseStr = request + "_STREAM_START";
    } else {
        responseStr = "Unknown request: " + request;
    }

    manageState();
    Logger::log("[RESPONSE] " + responseStr);
    
    // Return a response packet (type 2 to denote server response)
    return Packet(2, p.sequenceNumber, responseStr);
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
