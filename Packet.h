#ifndef PACKET_H
#define PACKET_H

#include <string>

class Packet {
public:
    Packet(int id, const std::string& data);
    ~Packet();

    int getId() const;
    std::string getData() const;

private:
    int id;
    std::string data;
};

#endif // PACKET_H
