#include "include/NetworkEngine.h"

void NetworkEngine::init() {
   if (this->socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
       std::cout << "Error setting up socket." << std::endl;
   }
   this->host = make_shared<Connection>(sf::IpAddress::getLocalAddress(), socket.getLocalPort());
   std::cout << "Ip Address is " << this->host->getIp().toString() << " (" << this->host->getIp().toInteger() << ")" << " on port " << this->host->getPort() << std::endl;
   this->connectionSetup();
   this->socket.setBlocking(false);
}


sf::Socket::Status NetworkEngine::sendPacket(sf::Packet *packet, sf::IpAddress receiverIp, unsigned short receiverPort) {
   sf::Socket::Status socketStatus;
   if ((socketStatus = this->socket.send(*packet, receiverIp, receiverPort)) == sf::Socket::Error) {
      std::cout << "Error sending packet to: " << receiverIp << ":" << receiverPort << std::endl;
   }
   return socketStatus;
}

void NetworkEngine::receivePacket(sf::Packet *packet) {
   if (socketStatus = this->socket.receive(*packet, this->host->getIp(), this->host->getPort())) != sf::Socket::Done) {
      std::cout << "Error receiving packet." << std::endl;
   }
}

void NetworkEngine::connectionSetup() {
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
      playerPort = (sf::Uint32)this->host->getPort();
      packet << (sf::Uint8)CONNECT_REQEST << playerIp << playerPort;

      std::cout << "Sending server " << serverIpString << ":" << serverPort << " with player info " << playerIp << ":" << playerPort << std::endl;

      this->sendPacket(&packet, serverIp, serverPort);
      //if (this->socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
         //std::cout << "Error sending connection." << std::endl;
      //}
      this->receivePacket(&packet, serverIp, serverPort);
      //if (this->socket.receive(packet, serverIp, serverPort) != sf::Socket::Done) {
         //std::cout << "Error receiving reply from server." << std::endl;
      //}

      packet >> flag;
      if ((FLAG)flag == CONNECT_ACCEPT) {
         packet >> this->playerId;
         this->server = make_shared<Connection>(serverIp, serverPort);
         connected = true;
      } else {
         std::cout << "Please Enter Server Information Again." << std::endl;
      }
   }
}

void NetworkEngine::execute(double delta_time) {
   if (SEND == status) {
      this->send();
      status = RECEIVE;
   } else if (RECEIVE == status) {
      this->receive();
      status = SEND;
   }
}

void NetworkEngine::send() {

}

void NetworkEngine::receive() {
   sf::Socket::Status socketStatus;
   sf::Packet packet;
   sf::Uint8 flag;

   if ((socketStatus = this->socket.receive(packet, this->host->getIp(), this->host->getPort())) == sf::Socket::Done) {
      packet >> flag;
      switch ((FLAG)flag) {
         case UPDATE_PLAYERS:
            this->updatePlayers(packet);
            break;
         case UPDATE_SHIP:
            this->updateShip(packet);
            break;
         case START_TRANSFORM:
            this->updateTransform(packet);
            break;
         case CONNECT_ACCEPT:
         case CONNECT_REQEST:
         case RECEIVE_PLAYER:
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


void NetworkEngine::updatePlayers(sf::Packet packet) {

}

void NetworkEngine::updateShip(sf::Packet packet) {

}

void NetworkEngine::updateTransform(sf::Packet packet) {
   sf::Uint8 flag;
   sf::Uint32 idx;
   std::shared_ptr<Networkable> cur;
   this->receivePacket(packet);
   packet >> flag;
   while ((FLAG)flag == UPDATE_TRANSFORM) {
      packet >> idx;
      cur = this->networkables.at(idx);
      cur->unpackTransform(packet);

      this->receivePacket(packet);
      packt >> flag;
   }
   if ((FLAG)flag != END_TRANSFORM) {
      std::cout << "Error recieving all Transform Updates" << std:endl;
   }
}

// FLAG packing packet overide operations
sf::Packet& operator <<(sf::Packet& packet, const NetworkEngine::FLAG& f) {
   return packet << (sf::Uint8)f;
}
sf::Packet& operator >>(sf::Packet& packet, NetworkEngine::FLAG& f) {
   sf::Uint8 temp;
   packet >> temp;
   f = (NetworkEngine::FLAG)temp;
   return packet;
}
