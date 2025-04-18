/*
** EPITECH PROJECT, 2025
** _
** File description:
** _
*/

#ifndef SOUNDMANAGER_HPP_
#define SOUNDMANAGER_HPP_

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void loadSound(const std::string &name, const std::string &filepath);
    void playSound(const std::string &name, bool loop = false);
    void stopSound(const std::string &name);
    bool isSoundPlaying(const std::string &name);

    void loadMusic(const std::string &filepath);
    void playMusic();
    void stopMusic();

    void setVolume(float soundVolume, float musicVolume);
    void stopAllSounds();

private:
    std::unordered_map<std::string, sf::SoundBuffer> mSoundBuffers;
    std::unordered_map<std::string, sf::Sound> mSounds;
    sf::Music mBackgroundMusic;
};

#endif /* SOUNDMANAGER_HPP_ */