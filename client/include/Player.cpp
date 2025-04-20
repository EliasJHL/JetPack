/*
** Player.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:51:49 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 14:36:55 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "Player.hpp"
#include "GameManager.hpp"

Player::Player(int id, std::string name)
    : mPlayerID(id), mPlayerName(name), mAnimator(mSprite, 134.5, 133.83, 6), mAnimationTimer(0.15f) // Initialisation du Timer
{
    if (!mTexture.loadFromFile("./client/ressources/sprites/player_sprite_sheet.png")) {
        throw std::runtime_error("Failed to load spritesheet");
    }
    mSprite.setTexture(mTexture);
    mAnimator.setFramesPerAction(4);

    mPos.x = 0;
    mPos.y = FLOOR;
    mSprite.setPosition(mPos);

    if (!mFont.loadFromFile("./client/ressources/font/jetpack_font.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    mScore = "0";
    mScoreText.setFont(mFont);
    mScoreText.setString("Score: 0");
    mScoreText.setCharacterSize(15);
    mScoreText.setFillColor(sf::Color::White);
    updateScoreText();
}

Player::~Player() {}

int Player::getID(void) const {
    return mPlayerID;
}

std::string Player::getName(void) const {
    return mPlayerName;
}

void Player::setName(std::string name) {
    mPlayerName = name;
}

std::pair<float, float> Player::getPosition(void) const {
    return {mPos.x, mPos.y};
}

void Player::updateOnlinePlayersPosition(std::pair<float, float> pos) 
{
    mPos.x = pos.first;
    mPos.y = pos.second;
    mSprite.setPosition(mPos);
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

sf::Sprite &Player::getSprite(void) {
    return mSprite;
}

void Player::updateAnimation() {
    if (mAnimationTimer.isElapsed()) {
        mAnimator.nextFrame();
        mAnimationTimer.restart();
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

void Player::updateScoreText() {
    mScoreText.setString("Score: " + mScore);
    mScoreText.setPosition(mPos.x - mScoreText.getGlobalBounds().width / 2, 50);
}

sf::Text &Player::getScoreText() {
    return mScoreText;
}

void Player::setScore(std::string score) {
    mScore = score;
    updateScoreText();
}

std::string Player::getScore() const {
    return mScore;
}