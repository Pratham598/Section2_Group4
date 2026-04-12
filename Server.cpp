#include "Server.h"
#include "Logger.h"

Server::Server() : currentState(ServerState::IDLE), isAuthenticated(false) 
{
  Logger::log("Server initialized in IDLE state.");
}

Server::~Server() {}

bool Server::authenticateUser(const std::string &username,
                              const std::string &password) {
  Logger::log("Authenticating user: " + username);
  if (username == "admin" && password == "password123") 
  {
    isAuthenticated = true;
    Logger::log("Authentication successful.");
    return true;
  }
  Logger::log("Authentication failed.");
  return false;
}

std::vector<Packet> Server::processRequest(const Packet &p) 
{
  Logger::logPacket("REQ", p);
  std::vector<Packet> responses;

  if (!isAuthenticated) 
  {
    Logger::log("Unauthorized request. Please authenticate first.");
    Packet response(2, p.sequenceNumber, "Unauthorized request.");
    Logger::logPacket("RES", response);
    responses.push_back(response);
    return responses;
  }

  std::string request = p.payload;
  Logger::log("Processing request: " + request);

  // State Machine Implementation
  switch (currentState) 
  {
  case ServerState::IDLE:
    if (request == "START_MONITOR") 
    {
      currentState = ServerState::MONITORING;
      responses.push_back(
          Packet(2, p.sequenceNumber, "Server state changed to MONITORING."));
    } else if (request == "STOP_MONITOR" || request == "GET_SNAPSHOT") {
      responses.push_back(Packet(2, p.sequenceNumber,
                                 "Error: Rejected invalid action '" + request +
                                     "' in IDLE state."));
    } else 
    {
      responses.push_back(Packet(2, p.sequenceNumber,
                                 "Error: Unknown request '" + request + "'."));
    }
    break;

  case ServerState::MONITORING:
    if (request == "STOP_MONITOR") 
    {
      currentState = ServerState::IDLE;
      responses.push_back(
          Packet(2, p.sequenceNumber, "Server state changed to IDLE."));
    } else if (request == "START_MONITOR") 
    {
      responses.push_back(Packet(
          2, p.sequenceNumber,
          "Error: Rejected invalid action. Server is already MONITORING."));
    } else if (request == "GET_SNAPSHOT")
     {
      // Simulate large image (>1MB)
      std::string largeImg(1500000, 'X'); // 1.5MB of 'X'
      size_t chunkSize = 500000;
      int totalChunks = (largeImg.size() + chunkSize - 1) / chunkSize;
      for (int i = 0; i < totalChunks; ++i) {
        std::string chunk = largeImg.substr(i * chunkSize, chunkSize);
        std::string chunkPrefix = "CHUNK:" + std::to_string(i + 1) + "/" +
                                  std::to_string(totalChunks) + ":";
        responses.push_back(
            Packet(2, p.sequenceNumber + i, chunkPrefix + chunk));
      }
    } else if (request.length() >= 7 && request.substr(0, 7) == "CAMERA_") 
    {
      responses.push_back(
          Packet(2, p.sequenceNumber, request + "_STREAM_START"));
    } else 
    {
      responses.push_back(Packet(2, p.sequenceNumber,
                                 "Error: Unknown request '" + request + "'."));
    }
    break;
  }

  manageState(); // Prints current state for debugging

  for (size_t i = 0; i < responses.size(); ++i) 
  {
    std::string logPayload = responses[i].payload;
    if (logPayload.length() > 60) {
      logPayload = logPayload.substr(0, 60) + "... [TRUNCATED FOR LOGGING]";
    }
    Logger::log("[RESPONSE " + std::to_string(i + 1) + "/" +
                std::to_string(responses.size()) + "] " + logPayload);

    Packet briefRes = responses[i];
    briefRes.payload = logPayload;
    Logger::logPacket("RES", briefRes);
  }

  return responses;
}

void Server::manageState() 
{
  switch (currentState) 
  {
  case ServerState::IDLE:
    Logger::log("[STATE] Server is currently IDLE. Waiting for commands.");
    break;
  case ServerState::MONITORING:
    Logger::log("[STATE] Server is currently MONITORING traffic cameras.");
    break;
  }
}
