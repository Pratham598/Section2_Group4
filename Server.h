#ifndef SERVER_H
#define SERVER_H

#include "Packet.h"
#include <string>

enum class ServerState {
    IDLE,
    MONITORING
};

class Server {
public:
    Server();
    ~Server();

    bool authenticateUser(const std::string& username, const std::string& password);
    void processRequest(const Packet& p);
    void manageState();
    void sendResponse(const std::string& response);

    // Simple loop method to simulate the server running
    void runLoop();

private:
    ServerState currentState;
    bool isAuthenticated;
};

#endif // SERVER_H
