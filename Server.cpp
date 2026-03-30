#include "Server.h"
#include "Logger.h"
#include <iostream>

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

void Server::processRequest(const Packet& p) {
    if (!isAuthenticated) {
        Logger::log("Unauthorized request. Please authenticate first.");
        return;
    }

    std::string request = p.payload;
    Logger::log("Processing request: " + request);

    if (request == "START_MONITOR") {
        if (currentState == ServerState::IDLE) {
            currentState = ServerState::MONITORING;
            sendResponse("Server state changed to MONITORING.");
            manageState();
        } else {
            sendResponse("Server is already monitoring.");
        }
    } else if (request == "STOP_MONITOR") {
        if (currentState == ServerState::MONITORING) {
            currentState = ServerState::IDLE;
            sendResponse("Server state changed to IDLE.");
            manageState();
        } else {
            sendResponse("Server is already idle.");
        }
    } else if (request == "GET_SNAPSHOT") {
        if (currentState == ServerState::MONITORING) {
            Logger::log("Generating snapshot...");
            for (int i = 1; i <= 3; ++i) {
                sendResponse("Sending image chunk " + std::to_string(i) + "/3...");
            }
            sendResponse("Snapshot transfer complete.");
        } else {
            sendResponse("Cannot get snapshot. Server is not in MONITORING state.");
        }
    } else {
        sendResponse("Unknown request: " + request);
    }
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

void Server::sendResponse(const std::string& response) {
    Logger::log("[RESPONSE] " + response);
}

void Server::runLoop() {
    Logger::log("Starting server simulation loop...");
    manageState();
}
