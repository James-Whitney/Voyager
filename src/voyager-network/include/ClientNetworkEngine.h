#pragma once
#ifndef VOYAGER_CLIENTNETWORKENGINE_H_INCLUDED
#define VOYAGER_CLIENTNETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "NetworkEngine.h"

#include <vector>
#include <iostream>

class ClientNetworkEngine : public NetworkEngine {
public:
   Connection *server;
   std::vector<Networkable> networkables;

   ClientNetworkEngine() : NetworkEngine::NetworkEngine(SEND) { }

   //void init();
   void execute(double delta_time);

   void connectionSetup();
   void send();
   void receive();
};

#endif
