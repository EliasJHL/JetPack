/*
** EPITECH PROJECT, 2025
** _
** File description:
** _
*/

#include "SoundManager.hpp"
#include <stdexcept>
#include <iostream>

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}

void SoundManager::loadSound(const std::string &name, const std::string &filepath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        throw std::runtime_error("Failed to load sound: " + filepath);
    }
    mSoundBuffers[name] = buffer;

    sf::Sound sound;
    sound.setBuffer(mSoundBuffers[name]);
    mSounds[name] = sound;
}

void SoundManager::playSound(const std::string &name, bool loop) {
    auto it = mSounds.find(name);
    if (it != mSounds.end()) {
        if (it->second.getStatus() != sf::Sound::Playing) {
            it->second.setLoop(loop);
            it->second.play();
        }
    } else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::stopSound(const std::string &name) {
    auto it = mSounds.find(name);
    if (it != mSounds.end()) {
        it->second.stop();
    } else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

bool SoundManager::isSoundPlaying(const std::string &name) {
    auto it = mSounds.find(name);
    if (it != mSounds.end()) {
        return it->second.getStatus() == sf::Sound::Playing;
    }
    return false;
}

void SoundManager::loadMusic(const std::string &filepath) {
    if (!mBackgroundMusic.openFromFile(filepath)) {
        throw std::runtime_error("Failed to load music: " + filepath);
    }
}

void SoundManager::playMusic() {
    mBackgroundMusic.setLoop(true);
    mBackgroundMusic.play();
}

void SoundManager::stopMusic() {
    mBackgroundMusic.stop();
}

void SoundManager::setVolume(float soundVolume, float musicVolume) {
    mBackgroundMusic.setVolume(musicVolume);
    for (auto &pair : mSounds) {
        pair.second.setVolume(soundVolume);
    }
}
