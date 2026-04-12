#include "Client.h"
#include "Server.h"
#include <iostream>

int main() {
    Server server;
    Client client("admin", "password123", server);
    
    // Start the interactive client menu
    client.menu();
    
    return 0;
}
