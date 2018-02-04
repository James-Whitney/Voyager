#include "include/ServerNetworkEngine.h"

void ServerNetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString() << " on port " << this->host->getPort() << std::endl;
}

enum {
   PLAYER_SETUP = -1,
   FIRST_PLAYER = -2,
   SEND_UPDATE = -3
}

void ServerNetworkEngine::execute(double delta_time) {
   if (CONNECTION_PHASE == delta_time) {
      ServerNetworkEngine::connectionSetup();
   } else if (SEND_UPDATE == delta_time) {
      ServerNetworkEngine::sendUpdate();
   } else if (RECEIVE_UPDATE == delta_time) {
      ServerNetworkEngine::receiveUpdate();
   }
}

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

   std::cout << "Player connected from IP " << senderAddress.toString() << " on port " << playerPort << std::endl;
}

void sendUpdate() {

}

void receiveUpdate() {

}
