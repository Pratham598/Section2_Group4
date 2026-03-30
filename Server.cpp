#include "Server.h"
#include "Logger.h"
#include <iostream>

Server::Server() : isRunning(false) {
    Logger::log("Server initialized.");
}

Server::~Server() {}

void Server::start() {
    isRunning = true;
    Logger::log("Server started.");
}

void Server::receivePacket(const Packet& packet) {
    if (!isRunning) {
        Logger::log("Server is not running. Packet dropped.");
        return;
    }
    Logger::log("Server received packet from Client " + std::to_string(packet.getId()) + 
                ": " + packet.getData());
}

void Server::stop() {
    isRunning = false;
    Logger::log("Server stopped.");
}
