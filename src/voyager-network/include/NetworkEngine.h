#pragma once
#ifndef VOYAGER_NETWORKENGINE_H_INCLUDED
#define VOYAGER_NETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "Connection.h"
#include "Networkable.h"

#include <voyager-core/include/Engine.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <bullet/src/btBulletDynamicsCommon.h>

#include <vector>
#include <iostream>

class NetworkEngine : public Engine {
public:
   Connection *host;
   sf::UdpSocket socket;
   int numPlayers;
   std::vector< std::shared_ptr<Connection> > players;
   std::vector< std::shared_ptr<Networkable> > networkables;

   enum STATUS { SEND, RECEIVE };
   STATUS status;

   enum FLAG {
      CONNECT_REQEST,
      CONNECT_ACCEPT,
      RECEIVE_PLAYER,
      UPDATE_PLAYERS,
      UPDATE_SHIP,
      UPDATE_TRANSFORM
   };

   NetworkEngine(STATUS s) { this->status = s; };

   void init();
   sf::Socket::Status sendPacket(sf::Packet *packet, sf::IpAddress receiverIp, unsigned short receiverPort);

   virtual void execute(double delta_time) = 0;

   virtual void connectionSetup() = 0;
   virtual void send() = 0;
   virtual void receive() = 0;
};

#endif
