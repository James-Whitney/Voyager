#pragma once
#ifndef VOYAGER_SERVERNETWORKENGINE_H_INCLUDED
#define VOYAGER_SERVERNETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "NetworkEngine.h"

#include <vector>
#include <iostream>

class ServerNetworkEngine : public NetworkEngine {
public:
   int numConnected = 1;

   sf::IpAddress allAddress = sf::IpAddress::Any;
   unsigned short allPorts = sf::Socket::AnyPort;

   int numPlayers;
   std::vector< std::shared_ptr<Connection> > players;

   ServerNetworkEngine() : NetworkEngine::NetworkEngine(RECEIVE) { }

   void execute(double delta_time);

   void connectionSetup();
   void send();
   void send(sf::Packet packet);
   void receive();

private:
   void addPlayer(sf::Packet packet);
   void updatePlayers(sf::Packet packet);
   void sendToPlayers(sf::Packet *packet);
   void sendNetworkableUpdates();
   void sendPlayerUpdates();
   void receivePlayer(sf::Packet packet);
};

#endif
