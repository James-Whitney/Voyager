#include "include/ServerNetworkEngine.h"
#include <glm/glm/gtc/type_ptr.hpp>

void ServerNetworkEngine::connectionSetup() {
   std::cout << "Number of Players? ";
   std::cin >> this->numPlayers;

   while (this->numConnected < this->numPlayers) {
      ServerNetworkEngine::receive();
   }
   for (auto &player : this->players) {

   }
}

void ServerNetworkEngine::execute(double delta_time) {
   if (SEND == status) {
      ServerNetworkEngine::send();
      status = RECEIVE;
   } else if (RECEIVE == status) {
      ServerNetworkEngine::receive();
      status = SEND;
   }
}


void ServerNetworkEngine::send() {
   ServerNetworkEngine::sendNetworkableUpdates();
   ServerNetworkEngine::sendPlayerUpdates();
}

void ServerNetworkEngine::sendNetworkableUpdates() {
   sf::Packet packet;
   std::shared_ptr<Networkable> networkable;

   for (int idx = 0; idx < this->networkables.size(); idx++) {
      networkable = this->networkables.at(idx);
      if (networkable->updateThis) {
         packet << UPDATE_TRANSFORM << idx;
         packet = networkable->packTransform(packet, networkable->getEntity()->getTransform());
         ServerNetworkEngine::sendToPlayers(&packet);
         packet.clear();
      }
   }
}

void ServerNetworkEngine::sendPlayerUpdates() {
   sf::Packet packet;
   std::shared_ptr<Connection> player;

   for (int idx = 0; idx < this->players.size(); idx++) {
      player = this->players.at(idx);
      if (player->getNetworkable()->updateThis) {
         packet << UPDATE_PLAYERS << idx;
         packet = player->getNetworkable()->packTransform(packet, player->getNetworkable()->getEntity()->getTransform());
         ServerNetworkEngine::sendToPlayers(&packet);
         packet.clear();
      }
   }
}

void ServerNetworkEngine::sendToPlayers(sf::Packet *packet) {
   for (auto &player : this->players) {
      NetworkEngine::sendPacket(packet, player->getIp(), player->getPort());
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
            ServerNetworkEngine::addPlayer(packet);
            break;
         case RECEIVE_PLAYER:
            ServerNetworkEngine::receivePlayer(packet);
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
   socketStatus = NetworkEngine::sendPacket(&packet, sf::IpAddress(playerIp), playerPort);
   if (socketStatus == sf::Socket::Done) { this->numConnected++; }
}

void ServerNetworkEngine::receivePlayer(sf::Packet packet) {
   sf::Uint8 id;
   std::shared_ptr<btTransform> player;

   packet >> id;
   player = this->players.at(id)->getNetworkable()->getEntity()->getTransform();

   this->players.at(id)->getNetworkable()->unpackTransform(packet, player);
   std::cout << "Received Player update from " << id << std::endl;
}
