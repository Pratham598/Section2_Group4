#include "Packet.h"

Packet::Packet(int id, const std::string& data) : id(id), data(data) {}

Packet::~Packet() {}

int Packet::getId() const {
    return id;
}

std::string Packet::getData() const {
    return data;
}
