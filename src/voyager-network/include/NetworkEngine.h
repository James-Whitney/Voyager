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

   enum {
      CONNECTION_PHASE = -1,
      SEND_UPDATE = -2,
      RECEIVE_UPDATE = -3
   }

   NetworkEngine() { fprintf(stderr, "setting up host\n\n"); };//host = new Connection(); };

   void init();
   void execute(double delta_time);

   virtual void sendUpdate() = 0;
   virtual void receiveUpdate() = 0;
   virtual void connectionSetup() = 0;
};

#endif
