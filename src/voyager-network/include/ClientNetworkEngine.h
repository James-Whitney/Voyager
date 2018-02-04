#pragma once
#ifndef VOYAGER_NETWORKENGINE_H_INCLUDED
#define VOYAGER_NETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "Connection.h"
#include "Networkable.h"
#include "NetworkEngine.h"

#include <vector>
#include <iostream>

class ClientNetworkEngine : public NetworkEngine {
public:
   Connection *server;
   std::vector<Networkable> networkables;

   ClientNetworkEngine() { fprintf(stderr, "setting up host\n\n"); };

   void init();
   void execute(double delta_time);
};

#endif
