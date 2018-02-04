#pragma once
#ifndef VOYAGER_NETWORKENGINE_H_INCLUDED
#define VOYAGER_NETWORKENGINE_H_INCLUDED

#include <SFML/Network.hpp>
#include "Connection.h"
#include "Networkable.h"
#include "NetworkEngine.h"

#include <vector>
#include <iostream>

class ServerNetworkEngine : public NetworkEngine {
public:
   std::vector<Connection> players;
   std::vector<Networkable> networkables;

   ServerNetworkEngine() { fprintf(stderr, "setting up host\n\n"); };//host = new Connection(); };

   void init();
   void execute(double delta_time);
};

#endif
