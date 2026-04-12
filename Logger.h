#ifndef LOGGER_H
#define LOGGER_H

#include "Packet.h"
#include <string>


class Logger {
public:
  static void log(const std::string &msg);
  static void logPacket(const std::string &direction, const Packet &p);
};

#endif