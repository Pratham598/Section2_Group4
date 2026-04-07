#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Packet.h"

class Server; // Forward declaration

class Client {
public:
    // Attributes
    Client(const std::string& username, const std::string& password, Server& server);
    ~Client();

    // Required methods
    void login();
    void sendRequest(const std::string& requestStr);
    void receiveResponse(const Packet& response);
    
    // Menu
    void menu();

private:
    std::string username;
    std::string password;
    Server& server;
};

#endif // CLIENT_H
