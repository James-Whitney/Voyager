// #include "include/Audio.h"

// void Audio::load(std::string key, std::string filename) {
//    std::shared_ptr<sf::SoundBuffer> buffer = std::make_shared();
//    if (!buffer->loadFromFile(filename)) {
//       std:::cout << "Error loading sound: " << filename << std::endl;
//       exit(1);
//    }

//    this->buffers->push_back(buffer);

//    std::shared_ptr<sf::Sound> sound;
//    sound->setBuffer(buffer);
//    this->sounds.insert({key, sound});
// }

// void Audio::play(std::string key, bool loop) {
//    std::shared_ptr<sf::Sound> sound = this->sounds[key];
//    sound->setLoop(loop);
//    sound->play();
// }

// void Audio::stop(std::string key) {
//    std::shared_ptr<sf::Sound> sound = this->sounds[key];
//    sound->stop();
// }