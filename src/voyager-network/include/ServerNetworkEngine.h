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

   ServerNetworkEngine();

   void init();
   void execute(double delta_time);

   void connectionSetup();
};

#endif
