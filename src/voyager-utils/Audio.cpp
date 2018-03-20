#include "include/Audio.h"

void Audio::load(std::string key, std::string filename) {
   std::shared_ptr<sf::SoundBuffer> buffer = std::make_shared<sf::SoundBuffer>();
   if (!buffer->loadFromFile(filename)) {
      std::cout << "Error loading sound: " << filename << std::endl;
      exit(1);
   }

   Audio::buffers.push_back(buffer);

   std::shared_ptr<sf::Sound> sound;
   sound->setBuffer(*buffer);
   Audio::sounds.insert({key, sound});
}

void Audio::play(std::string key, bool loop) {
   std::shared_ptr<sf::Sound> sound = Audio::sounds[key];
   sound->setLoop(loop);
   sound->play();
}

void Audio::stop(std::string key) {
   std::shared_ptr<sf::Sound> sound = Audio::sounds[key];
   sound->stop();
}