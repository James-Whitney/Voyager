#include "include/ClientNetworkEngine.h"

void ClientNetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = new Connection(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString() << " on port " << this->host->getPort() << std::endl;
}

void ClientNetworkEngine::execute(double delta_time) {
   if (CONNECTION_PHASE == delta_time) {
      ClientNetworkEngine::connectionSetup();
   } else if (SEND_UPDATE == delta_time) {
      ClientNetworkEngine::sendUpdate();
   } else if (RECEIVE_UPDATE == delta_time) {
      ClientNetworkEngine::sendUpdate();
   }
}

void ClientNetworkEngine::connectionSetup() {
   sf::IpAddress serverIp;
   std::string serverIpString;
   unsigned short serverPort;

   sf::Uint32 playerIp;
   unsigned short playerPort;

   sf::Packet packet;

   bool connected = false;

   while (!connected) {
      std::cout << "Server IP: ";
      std::cin >> serverIpString;
      serverIp = sf::IpAddress(serverIpString);
      std::cout << "Server Port: ";
      std::cin >> serverPort;

      playerIp = this->host->getIp().toInteger();
      playerPort = this->host->getPort();
      packet << playerIp << playerPort;

      if (this->socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
         std::cout << "Error sending connection." << std::endl;
      }
      if (this->socket.receive(packet, serverIp, serverPort) != sf::Socket::Done) {
         std::cout << "Error receiving reply from server." << std::endl;
      }

      packet >> connected;

      if (connected) {
         this->server = new Connection(serverIp, serverPort);
      } else {
         std::cout << "Please Enter Server Information Again." << endl;
      }
   }
}


void sendUpdate() {

}

void receiveUpdate() {

}
