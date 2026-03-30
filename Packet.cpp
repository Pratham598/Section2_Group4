#include "Packet.h"
#include <stdexcept>

// Default constructor
Packet::Packet() : packetType(0), sequenceNumber(0), payloadSize(0), payload("") {}

// Parameterized constructor
Packet::Packet(int type, int seqNum, const std::string& data) 
    : packetType(type), sequenceNumber(seqNum), payload(data) {
    payloadSize = static_cast<int>(data.length());
}

// Encodes the packet into a string containing the header and payload
std::string Packet::encode() const {
    // Header format: Type|SeqNum|Size|
    std::string header = std::to_string(packetType) + "|" +
                         std::to_string(sequenceNumber) + "|" +
                         std::to_string(payloadSize) + "|";
    
    // Payload can be dynamic text or binary (like image chunks)
    return header + payload;
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

    // Extract payloadSize
    nextPos = data.find('|', pos);
    if (nextPos == std::string::npos) throw std::invalid_argument("Invalid packet: missing payloadSize");
    payloadSize = std::stoi(data.substr(pos, nextPos - pos));
    pos = nextPos + 1;

    // Extract payload based on parsed payloadSize
    if (payloadSize > 0 && pos + payloadSize <= data.length()) {
        payload = data.substr(pos, payloadSize);
    } else if (payloadSize > 0 && pos < data.length()) {
        // If data is truncated slightly, read what remains
        payload = data.substr(pos);
    } else {
        payload = "";
    }
}
