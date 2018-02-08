#pragma once
#ifndef VOYAGER_SERVERNETWORKENGINE_H_INCLUDED
#define VOYAGER_SERVERNETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "NetworkEngine.h"

#include <vector>
#include <iostream>

class ServerNetworkEngine : public NetworkEngine {
public:
   std::vector<Connection> players;
   std::vector<Networkable> networkables;
   
   sf::IpAddress allAddress = sf::IpAddress::Any;
   unsigned short allPorts = 0;

   ServerNetworkEngine();

   //void init();
   void execute(double delta_time);

   void connectionSetup();
   void send();
   void receive();
   void updateTransform(sf::Packet packet);
};

#endif
