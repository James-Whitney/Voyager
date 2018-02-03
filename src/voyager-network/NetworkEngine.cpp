#include "include/NetworkEngine.h"

void NetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString() << " on port " << this->host->getPort() << std::endl;
}

void NetworkEngine::execute(double delta_time) {
   if (-1 == delta_time) {
      NetworkEngine::connectToServer();
   } else if (-2 == delta_time) {
      NetworkEngine::waitForFirstPlayer();
   } else {

   }
}

void NetworkEngine::connectToServer() {
   sf::IpAddress recipient;
   std::string serverIp;
   unsigned short serverPort;

   std::cout << "Server IP: ";
   std::cin >> serverIp;
   recipient = sf::IpAddress(serverIp);

   std::cout << "Server Port: ";
   std::cin >> serverPort;

   sf::Uint32 playerIp = this->host->getIp().toInteger();
   unsigned short playerPort = this->host->getPort();

   sf::Packet packet;
   packet << playerIp << playerPort;

   if (this->socket.send(packet, recipient, serverPort) != sf::Socket::Done) {
      std::cout << "Error sending connection." << std::endl;
   }
}

void NetworkEngine::waitForFirstPlayer() {
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
