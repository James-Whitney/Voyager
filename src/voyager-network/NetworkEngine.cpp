#include "include/NetworkEngine.h"

void NetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
   {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host.getIp().toString() << " on port " << this->host.getPort() << std::endl;
}
