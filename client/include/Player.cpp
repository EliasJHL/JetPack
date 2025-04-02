/*
** Player.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:51:49 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 23:30:25 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "Player.hpp"
#include "GameManager.hpp"

Player::Player(int id, std::string name)
    : mPlayerID(id), mPlayerName(name), mAnimator(mSprite, 134.5, 133.83, 6) // Initialize mAnimator
{
    if (!mTexture.loadFromFile("./client/ressources/Sprites/player_sprite_sheet.png")) {
        throw std::runtime_error("Failed to load spritesheet");
    }
    mSprite.setTexture(mTexture);
    mSprite.setScale(1.0, 1.0);
    mAnimator.setFramesPerAction(4); // Assuming 4 frames per action
    mPos.x = 350;
    mPos.y = 570;
    mSprite.setPosition(mPos);
}

Player::~Player() {}

int Player::getID(void) const {
    return mPlayerID;
}

std::string Player::getName(void) const {
    return mPlayerName;
}

std::pair<float, float> Player::getPosition(void) const {
    return {mPos.x, mPos.y};
}

void Player::setPosition(std::pair<float, float> pos) {
    mPos.x = pos.first;
    mPos.y = pos.second;
    if (mPos.y > FLOOR)
        mPos.y = FLOOR;
    if (mPos.y < CEILING)
        mPos.y = CEILING;
    mSprite.setPosition(mPos);
}

sf::Sprite Player::getPlayerSprite(void) {
    return mSprite;
}

void Player::updateAnimation() {
    if (mAnimationClock.getElapsedTime().asSeconds() >= mAnimationInterval) { // Change frame every 0.3 seconds
        mAnimator.nextFrame();
        mAnimationClock.restart(); // Restart the clock
    }
    mSprite.setTextureRect(mAnimator.getTextureRect());
}

void Player::setAction(int action, int mode) {
    // si action == 1 et que mDefaultMode == true, on fait
    if (mode == 0) {
        mAnimator.setDefaultAction(action);
    } else if (mode == 1) {
        mAnimator.setOneAction(action, 0);
    } else {
        mAnimator.setAction(action);
    }
}
