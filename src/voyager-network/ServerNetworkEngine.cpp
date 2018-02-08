#include "include/ServerNetworkEngine.h"

ServerNetworkEngine::ServerNetworkEngine() : NetworkEngine() {
   fprintf(stderr, "setting up host\n\n");
}

void ServerNetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString() << " on port " << this->host->getPort() << std::endl;
   ServerNetworkEngine::connectionSetup();
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

   std::cout << "Player connected from IP " << senderAddress.toString() << " on port "
      << playerPort << std::endl;

   packet << true;
   if (this->socket.send(packet, senderAddress, senderPort) != sf::Socket::Done) {
      std::cout << "Error sending response." << std::endl;
   }
}

void ServerNetworkEngine::execute(double delta_time) {
   std::cout << "Execute Server Network Engine." << std::endl;
}
