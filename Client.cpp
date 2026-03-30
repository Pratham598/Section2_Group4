#include "Client.h"
#include "Server.h"
#include "Logger.h"

Client::Client(int id, Server& server) : id(id), server(server) {
    Logger::log("Client " + std::to_string(id) + " created.");
}

Client::~Client() {}

void Client::sendData(const std::string& data) {
    Logger::log("Client " + std::to_string(id) + " sending data...");
    Packet packet(id, data);
    server.receivePacket(packet);
}

int Client::getId() const {
    return id;
}
