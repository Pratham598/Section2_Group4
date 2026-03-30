#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Packet.h"

class Server; // Forward declaration

class Client {
public:
    Client(int id, Server& server);
    ~Client();

    void sendData(const std::string& data);
    int getId() const;

private:
    int id;
    Server& server;
};

#endif // CLIENT_H
