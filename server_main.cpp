#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include "Server.h"
#include "Packet.h"
#include "Logger.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT "8080"
#define DEFAULT_BUFLEN 4096

// Global server instance
Server g_server;

void handleClient(SOCKET clientSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
    
    if (iResult > 0) {
        recvbuf[iResult] = '\0';
        std::string request(recvbuf);
        
        // DEBUG: Print raw request to identify potential issues
        std::cout << "\n--- Incoming Request ---\n" << request << "\n------------------------" << std::endl;
        
        // Basic HTTP Parsing
        size_t firstSpace = request.find(' ');
        size_t secondSpace = request.find(' ', firstSpace + 1);
        
        if (firstSpace != std::string::npos && secondSpace != std::string::npos) {
            std::string method = request.substr(0, firstSpace);
            std::string path = request.substr(firstSpace + 1, secondSpace - firstSpace - 1);
            
            // Handle CORS Preflight (OPTIONS request)
            if (method == "OPTIONS") {
                std::string httpResponse = 
                    "HTTP/1.1 204 No Content\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Allow-Methods: GET, OPTIONS\r\n"
                    "Access-Control-Allow-Headers: Content-Type\r\n"
                    "Connection: close\r\n\r\n";
                send(clientSocket, httpResponse.c_str(), (int)httpResponse.length(), 0);
                closesocket(clientSocket);
                return;
            }

            std::string apiCommand = "";
            if (path == "/start") apiCommand = "START_MONITOR";
            else if (path == "/stop") apiCommand = "STOP_MONITOR";
            else if (path == "/snapshot") apiCommand = "GET_SNAPSHOT";
            
            if (!apiCommand.empty()) {
                std::cout << "[SERVER] Received request: " << apiCommand << std::endl;
                
                Packet p(1, 1, apiCommand);
                g_server.processRequest(p);
                
                std::string statusText = "UNKNOWN";
                if (apiCommand == "START_MONITOR") {
                    statusText = "MONITORING";
                    std::cout << "[SERVER] State changed to MONITORING" << std::endl;
                } else if (apiCommand == "STOP_MONITOR") {
                    statusText = "IDLE";
                    std::cout << "[SERVER] State changed to IDLE" << std::endl;
                } else if (apiCommand == "/snapshot" || apiCommand == "GET_SNAPSHOT") {
                     statusText = "MONITORING";
                }
                
                std::string responseBody = "{\"status\": \"" + statusText + "\", \"message\": \"Success\", \"image\": \"camera_feed_sample.png\"}";
                
                std::string httpResponse = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: " + std::to_string(responseBody.length()) + "\r\n"
                    "Connection: close\r\n"
                    "\r\n" + responseBody;
                    
                send(clientSocket, httpResponse.c_str(), (int)httpResponse.length(), 0);
            } else {
                std::string responseBody = "{\"error\": \"Not Found\"}";
                std::string httpResponse = 
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: " + std::to_string(responseBody.length()) + "\r\n"
                    "Connection: close\r\n"
                    "\r\n" + responseBody;
                send(clientSocket, httpResponse.c_str(), (int)httpResponse.length(), 0);
            }
        }
    }
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKET listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[SERVER] Listening on port 8080" << std::endl;

    g_server.authenticateUser("admin", "password123"); 

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            handleClient(clientSocket);
        }
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}