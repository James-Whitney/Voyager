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
   std::shared_ptr<Connection> host;
   std::shared_ptr<Connection> server;
   sf::UdpSocket socket;

   int playerId;
   std::vector< std::shared_ptr<Networkable> > networkables;

   enum STATUS { SEND, RECEIVE };
   STATUS status;

   enum FLAG {
      CONNECT_REQEST,
      CONNECT_ACCEPT,
      RECEIVE_PLAYER,
      UPDATE_PLAYERS,
      UPDATE_SHIP,
      UPDATE_TRANSFORM,
      START_TRANSFORM,
      END_TRANSFORM
   };

   NetworkEngine(STATUS s = SEND) { this->status = s; };

   void init();
   sf::Socket::Status sendPacket(sf::Packet *packet, sf::IpAddress receiverIp, unsigned short receiverPort);

   void execute(double delta_time);

   void connectionSetup();
   void send();
   void receive();

private:
   void updatePlayers(sf::Packet packet);
   void updateShip(sf::Packet packet);
   void updateTransform(sf::Packet packet);
   void receivePacket(sf::Packet &packet, sf::IpAddress serverIp, unsigned short serverPort);
   void receivePacket(sf::Packet &packet);
};

#endif
