#include "include/NetworkEngine.h"

void NetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString()
      << " on port " << this->host->getPort() << std::endl;
   NetworkEngine::connectionSetup();
   this->socket.setBlocking(false);
}


sf::Socket::Status NetworkEngine::sendPacket(sf::Packet *packet, sf::IpAddress receiverIp, unsigned short receiverPort) {
   sf::Socket::Status socketStatus;
   if ((socketStatus = this->socket.send(*packet, receiverIp, receiverPort)) == sf::Socket::Error) {
      std::cout << "Error sending packet." << std::endl;
   }
   return socketStatus;
}
