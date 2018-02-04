#include "include/NetworkEngine.h"

void NetworkEngine::init() {
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

void NetworkEngine::execute(double delta_time) {
   if (PLAYER_SETUP == delta_time) {
      NetworkEngine::connectToServer();
   } else if (FIRST_PLAYER == delta_time) {
      NetworkEngine::waitForFirstPlayer();
   } else if (SEND_UPDATE == delta_time) {
      
   }
}

void NetworkEngine::connectToServer() {
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
