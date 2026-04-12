#include "Logger.h"
#include <iostream>

void Logger::log(const std::string &msg) { std::cout << msg << std::endl; }

void Logger::logPacket(const std::string &direction, const Packet &p) {
  std::cout << "[" << direction << "] "
            << "Type=" << p.packetType << ", Seq=" << p.sequenceNumber
            << ", Size=" << p.payload.size() << std::endl;
}