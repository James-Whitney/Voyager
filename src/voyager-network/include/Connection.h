#pragma once
#ifndef VOYAGER_CONNECTION_H_INCLUDED
#define VOYAGER_CONNECTION_H_INCLUDED

#include <SFML/Network.hpp>

#include "Networkable.h"

class Connection {
private:
   sf::IpAddress ip;
   unsigned short port;
   std::shared_ptr<Networkable> networkable;
public:
   Connection(sf::IpAddress addr = 0, unsigned short p = 0) {
      this->ip = addr;
      this->port = p;
   }

   sf::IpAddress getIp() { return this->ip; }
   unsigned short getPort() { return this->port; }

   void setNetworkable(std::shared_ptr<Networkable> n) { this->networkable = n; }
   std::shared_ptr<Networkable> getNetworkable() { return this->networkable; }
};

#endif
