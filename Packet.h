#ifndef PACKET_H
#define PACKET_H

#include <string>

class Packet {
public:
    int packetType;
    int sequenceNumber;
    int payloadSize;
    std::string payload;

    // Constructors
    Packet();
    Packet(int type, int seqNum, const std::string& data);

    // Core functionality
    std::string encode() const;
    void decode(const std::string& data);
};

#endif // PACKET_H
