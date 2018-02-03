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

   NetworkEngine() { fprintf(stderr, "setting up host\n\n"); };//host = new Connection(); };

   void init();
   void execute(double delta_time);
   void connectToServer();
   void waitForFirstPlayer();
};

#endif
