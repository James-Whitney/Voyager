#pragma once
#ifndef VOYAGER_NETWORKENGINE_H_INCLUDED
#define VOYAGER_NETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "Connection.h"
#include "Networkable.h"

#include <voyager-core/include/Engine.h>

#include <vector>
#include <iostream>

class NetworkEngine : public Engine {
public:
   Connection *host;
   sf::UdpSocket socket;
   std::vector<Connection> players;
   std::vector<Networkable> networkables;

   enum STATUS { SEND, RECEIVE };
   STATUS status;

   enum PACKET {
      PLAYER,
      TRANSFORM,
      CONNECTION,
      TRANSFORM_LIST
   };

   NetworkEngine() { };

   //virtual void init() = 0;
   void init() {
      if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
          std::cout << "Error setting up socket." << std::endl;
      }
      this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
      std::cout << "Ip Address is " << this->host->getIp().toString()
         << " on port " << this->host->getPort() << std::endl;
      connectionSetup();
      this->socket.setBlocking(false);
   }
   virtual void execute(double delta_time) = 0;

   virtual void connectionSetup() = 0;
   virtual void send() = 0;
   virtual void receive() = 0;
};

#endif
