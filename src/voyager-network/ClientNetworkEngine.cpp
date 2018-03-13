#include "include/ClientNetworkEngine.h"

void ClientNetworkEngine::connectionSetup() {
   sf::IpAddress serverIp;
   std::string serverIpString;
   unsigned short serverPort;

   sf::Uint32 playerIp;
   sf::Uint32 playerPort;

   sf::Packet packet;

   sf::Uint8 flag;
   bool connected = false;

   while (!connected) {
      std::cout << "Server IP: ";
      std::cin >> serverIpString;
      serverIp = sf::IpAddress(serverIpString);
      std::cout << "Server Port: ";
      std::cin >> serverPort;

      playerIp = this->host->getIp().toInteger();
      playerPort = (sf::Uint32)this->host->getPort(); //std::to_string(i)
      packet << (sf::Uint8)CONNECT_REQEST << playerIp << playerPort;

      std::cout << "Sending server " << serverIpString << ":" << serverPort << " with player info " << playerIp << ":" << playerPort << std::endl;

      if (this->socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
         std::cout << "Error sending connection." << std::endl;
      }
      if (this->socket.receive(packet, serverIp, serverPort) != sf::Socket::Done) {
         std::cout << "Error receiving reply from server." << std::endl;
      }

      packet >> flag;

      if ((FLAG)flag == CONNECT_ACCEPT) {
         packet >> this->numPlayers;
         this->server = make_shared<Connection>(serverIp, serverPort);
         connected = true;
      } else {
         std::cout << "Please Enter Server Information Again." << std::endl;
      }
   }
}

void ClientNetworkEngine::execute(double delta_time) {
   if (SEND == status) {
      ClientNetworkEngine::send();
      status = RECEIVE;
   } else if (RECEIVE == status) {
      ClientNetworkEngine::receive();
      status = SEND;
   }
}

void ClientNetworkEngine::send() {

}

void ClientNetworkEngine::receive() {

}
