#include "Client.h"
#include "Server.h"
#include "Logger.h"

Client::Client(int id, Server& server) : id(id), server(server) {
    Logger::log("Client " + std::to_string(id) + " created.");
}

Client::~Client() {}

void Client::sendRequest(const std::string& request) {
    Logger::log("Client " + std::to_string(id) + " sending request: " + request);
    // Use the 3 arg Packet constructor we pulled from remote
    Packet packet(1, 1, request); 
    server.processRequest(packet);
}

int Client::getId() const {
    return id;
}
