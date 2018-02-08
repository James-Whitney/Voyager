#include "include/ServerNetworkEngine.h"
#include <voyager-utils/include/Transform.h>
#include <glm/glm/gtc/type_ptr.hpp>

ServerNetworkEngine::ServerNetworkEngine() : NetworkEngine::NetworkEngine() {
   status = RECEIVE;
}

/*
void ServerNetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString()
      << " on port " << this->host->getPort() << std::endl;
   ServerNetworkEngine::connectionSetup();
   this->socket.setBlocking(false);
}
*/

void ServerNetworkEngine::connectionSetup() {
   sf::Packet packet;
   sf::IpAddress senderAddress;
   unsigned short senderPort;

   sf::Uint32 playerIp;
   unsigned short playerPort;

   if (this->socket.receive(packet, senderAddress, senderPort) != sf::Socket::Done) {
      std::cout << "Error sending connection." << std::endl;
   }

   packet >> playerIp >> playerPort;

   senderAddress = sf::IpAddress(playerIp);

   std::cout << "Player connected from IP " << senderAddress.toString() << " on port "
      << playerPort << std::endl;

   packet << true;
   if (this->socket.send(packet, senderAddress, senderPort) != sf::Socket::Done) {
      std::cout << "Error sending response." << std::endl;
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
   sf::Packet packet;
   std::shared_ptr<Transform> t;
   glm::vec3 scale, pos;
   //float pitch, yaw, roll;

   for (auto &networkable : networkables) {
      if (networkable.updateThis) {
         t = networkable.prevTransform;
         scale = t->getScale();
         pos = t->getPosition();
         packet << (sf::Uint8)TRANSFORM << (sf::Uint32)networkable.getId() << scale.x << scale.y << scale.z
            << pos.x << pos.y << pos.z << t->getPitch() << t->getYaw() << t->getRoll();
         for (auto &player : players) {
            if (this->socket.send(packet, player.getIp(), player.getPort())
               != sf::Socket::Done) {
               std::cout << "Error sending updated transform packet to "
                  << player.getIp() << ":" << player.getPort() << std::endl;
            }
         }
      }
   }
}

void ServerNetworkEngine::receive() {
   sf::Socket::Status socketStatus;
   sf::Packet packet;
   sf::Uint8 type;

   while ((socketStatus = this->socket.receive(packet, this->allAddress, this->allPorts))
      == sf::Socket::Done) {
      packet >> type;
      switch ((int)type) {
         case PLAYER:

         break;
         case TRANSFORM:
            ServerNetworkEngine::updateTransform(packet);
         break;
         case CONNECTION:

         break;
         case TRANSFORM_LIST:

         break;
      }
   }
}

void ServerNetworkEngine::updateTransform(sf::Packet packet) {
   sf::Uint32 id;
   float scale_x, scale_y, scale_z, pos_x, pos_y, pos_z, pitch, yaw, roll;
   packet >> id;

   Entity *entity = NULL;//NetworkEngine::getEntity(id);

}
