#pragma once
#ifndef VOYAGER_CONNECTION_H_INCLUDED
#define VOYAGER_CONNECTION_H_INCLUDED

#include <SFML/Network.hpp>

class Connection {
private:
   sf::IpAddress ip;
   unsigned short port;
public:
   Connection(sf::IpAddress addr = 0, unsigned short p = 0) { ip = addr; port = p; }
   sf::IpAddress getIp() { return ip; }
   unsigned short getPort() { return port; }
};

#endif
