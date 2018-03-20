#pragma once
#ifndef _AUDIO_H
#define _AUDIO_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include <SFML/Audio.hpp>

class Audio {

public:

   void static load(std::string key, std::string filename);

   void static play(std::string key, bool loop);
   void static stop(std::string key);

private:

   Audio() {}

   static std::unordered_map<std::string, std::shared_ptr<sf::Sound>> sounds;
   static std::vector<std::shared_ptr<sf::SoundBuffer>> buffers;

};

#endif