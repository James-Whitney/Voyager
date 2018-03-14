#include "include/ServerNetworkEngine.h"
#include <glm/glm/gtc/type_ptr.hpp>

void ServerNetworkEngine::connectionSetup() {
   this->players.push_back(this->host);

   std::cout << "Number of Players? ";
   std::cin >> this->numPlayers;

   while (this->numConnected < this->numPlayers) {
      this->receive();
   }
   for (auto &player : this->players) {

   }
}

void ServerNetworkEngine::send() {
   this->sendNetworkableUpdates();
   this->sendPlayerUpdates();
}

void ServerNetworkEngine::sendNetworkableUpdates() {
   sf::Packet packet;
   std::shared_ptr<Networkable> networkable;
   packet << START_TRANSFORM;


   for (int idx = 0; idx < this->networkables.size(); idx++) {
      networkable = this->networkables.at(idx);
      if (networkable->updateThis) {
         packet << UPDATE_TRANSFORM << (sf::Uint32)idx;
         packet = networkable->packTransform(packet);
         this->sendToPlayers(&packet);
         packet.clear();
      }
   }
   packet << END_TRANSFORM;

}

void ServerNetworkEngine::sendPlayerUpdates() {
   sf::Packet packet;
   std::shared_ptr<Connection> player;

   for (int idx = 1; idx < this->players.size(); idx++) {
      player = this->players.at(idx);
      if (player->getNetworkable()->updateThis) {
         packet << UPDATE_PLAYERS << (sf::Uint32)idx;
         packet = player->getNetworkable()->packTransform(packet);
         this->sendToPlayers(&packet);
         packet.clear();
      }
   }
}

void ServerNetworkEngine::sendToPlayers(sf::Packet *packet) {
   for (auto &player : this->players) {
      this->sendPacket(packet, player->getIp(), player->getPort());
   }
}

void ServerNetworkEngine::receive() {
   sf::Socket::Status socketStatus;
   sf::Packet packet;
   sf::Uint8 flag;

   while ((socketStatus = this->socket.receive(packet, this->allAddress, this->allPorts)) == sf::Socket::Done) {
      packet >> flag;
      switch ((FLAG)flag) {
         case CONNECT_REQEST:
            this->addPlayer(packet);
            break;
         case RECEIVE_PLAYER:
            this->receivePlayer(packet);
            break;
         case CONNECT_ACCEPT:
         case UPDATE_PLAYERS:
         case UPDATE_SHIP:
         case UPDATE_TRANSFORM:
         default:
            std::cout << "Bad Packet Flag Recieved" << std::endl;
            std::cout << "Recieved Flag: " << (FLAG)flag << std::endl;
            break;
      }
   }
   if (socketStatus == sf::Socket::Error) {
      std::cout << "Error receiving packet." << std::endl;
   }
}

void ServerNetworkEngine::addPlayer(sf::Packet packet) {
   sf::Uint32 playerIp;
   sf::Uint32 playerPort;
   sf::Socket::Status socketStatus;

   packet >> playerIp >> playerPort;
   this->players.push_back(std::make_shared<Connection>(sf::IpAddress(playerIp), (unsigned short)playerPort));

   std::cout << "Player " << this->numConnected << " connected from IP "
      << this->players.at(this->numConnected)->getIp().toString()
      << " on port " << playerPort << std::endl;

   packet.clear();
   packet << (sf::Uint8)CONNECT_ACCEPT << this->numConnected;
   socketStatus = this->sendPacket(&packet, sf::IpAddress(playerIp), playerPort);
   if (socketStatus == sf::Socket::Done) { this->numConnected++; }
}

void ServerNetworkEngine::receivePlayer(sf::Packet packet) {
   sf::Uint8 id;
   std::shared_ptr<btTransform> player;

   packet >> id;
   player = this->players.at(id)->getNetworkable()->getEntity()->getTransform();

   this->players.at(id)->getNetworkable()->unpackTransform(packet);
   std::cout << "Received Player update from " << id << std::endl;
}
