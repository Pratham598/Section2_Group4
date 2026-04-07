#ifndef SERVER_H
#define SERVER_H

#include "Packet.h"
#include <string>

#include <vector>

enum class ServerState {
    IDLE,
    MONITORING
};

class Server {
public:
    Server();
    ~Server();

    bool authenticateUser(const std::string& username, const std::string& password);
    std::vector<Packet> processRequest(const Packet& p);
    void manageState();

private:
    ServerState currentState;
    bool isAuthenticated;
};

#endif // SERVER_H
