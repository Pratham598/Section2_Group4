#include "Packet.h"
#include <stdexcept>

// Default constructor
Packet::Packet() : packetType(0), sequenceNumber(0), payload("") {}

// Parameterized constructor
Packet::Packet(int type, int seqNum, const std::string& data) 
    : packetType(type), sequenceNumber(seqNum), payload(data) {}

// Encodes the packet into a string containing the header and payload
std::string Packet::encode() const {
    // Header format: Type|SeqNum|
    return std::to_string(packetType) + "|" +
           std::to_string(sequenceNumber) + "|" +
           payload;
}

// Decodes the string back into Packet fields
void Packet::decode(const std::string& data) {
    size_t pos = 0;
    size_t nextPos = 0;

    // Extract packetType
    nextPos = data.find('|', pos);
    if (nextPos == std::string::npos) throw std::invalid_argument("Invalid packet: missing packetType");
    packetType = std::stoi(data.substr(pos, nextPos - pos));
    pos = nextPos + 1;

    // Extract sequenceNumber
    nextPos = data.find('|', pos);
    if (nextPos == std::string::npos) throw std::invalid_argument("Invalid packet: missing sequenceNumber");
    sequenceNumber = std::stoi(data.substr(pos, nextPos - pos));
    pos = nextPos + 1;

    // Remaining string is the payload
    if (pos < data.length()) {
        payload = data.substr(pos);
    } else {
        payload = "";
    }
}
