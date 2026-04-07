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

std::string Server::processRequest(const Packet& p) {
    if (!isAuthenticated) {
        Logger::log("Unauthorized request. Please authenticate first.");
        return "Unauthorized request.";
    }

    std::string request = p.payload;
    Logger::log("Processing request: " + request);
    std::string response;

    if (request == "START_MONITOR") {
        if (currentState == ServerState::IDLE) {
            currentState = ServerState::MONITORING;
            response = "Server state changed to MONITORING.";
        } else {
            response = "Server is already monitoring.";
        }
    } else if (request == "STOP_MONITOR") {
        if (currentState == ServerState::MONITORING) {
            currentState = ServerState::IDLE;
            response = "Server state changed to IDLE.";
        } else {
            response = "Server is already idle.";
        }
    } else if (request == "GET_SNAPSHOT") {
        if (currentState == ServerState::MONITORING) {
            response = "Snapshot transfer complete.";
        } else {
            response = "Cannot get snapshot. Server is not in MONITORING state.";
        }
    } else if (request.length() >= 7 && request.substr(0, 7) == "CAMERA_") {
        currentState = ServerState::MONITORING;
        response = request + "_STREAM_START";
    } else {
        response = "Unknown request: " + request;
    }

    manageState();
    Logger::log("[RESPONSE] " + response);
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
