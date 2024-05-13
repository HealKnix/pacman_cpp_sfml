struct GameSound {
  sf::SoundBuffer soundBuffer;
  sf::Sound sound;

  GameSound() {}

  GameSound(std::string soundName, short int volume) {
    if (!soundBuffer.loadFromFile(soundName)) return;
    sound.setBuffer(soundBuffer);
    sound.setVolume(volume);
  }

  void play() { sound.play(); }
};