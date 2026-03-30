#ifndef SERVER_H
#define SERVER_H

#include "Packet.h"

class Server {
public:
    Server();
    ~Server();

    void start();
    void receivePacket(const Packet& packet);
    void stop();

private:
    bool isRunning;
};

#endif // SERVER_H
