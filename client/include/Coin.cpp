#include "Coin.hpp"

Coin::Coin() : mAnimator(mSprite, 191.933, 171, 1), mAnimationTimer(0.15f) {
    if (!mTexture.loadFromFile("./client/ressources/sprites/coins_sprite_sheet.png")) {
        throw std::runtime_error("Failed to load coin spritesheet");
    }
    mSprite.setTexture(mTexture);
    mSprite.setScale(0.5, 0.5);
    mAnimator.setFramesPerAction(6);
    mAnimator.setDefaultAction(0);
    mPos.x = 400;
    mPos.y = 450;
    mSprite.setPosition(mPos);
    mToDisplay = true;
}

void Coin::setPosition(std::pair<float, float> pos) {
    mPos.x = pos.first;
    mPos.y = pos.second;
    mSprite.setPosition(mPos);
}

std::pair<float, float> Coin::getPosition() const {
    return {mPos.x, mPos.y};
}

void Coin::updateAnimation() {
    if (mAnimationTimer.isElapsed()) {
        mAnimator.nextFrame();
        mAnimationTimer.restart();
    }
    mSprite.setTextureRect(mAnimator.getTextureRect());
}

sf::Sprite &Coin::getSprite() {
    if (mToDisplay)
        mSprite.setColor(sf::Color(255, 255, 255, 255));
    else
        mSprite.setColor(sf::Color(255, 255, 255, 0));
    return mSprite;
}

void Coin::toDisplay(bool to_display)
{
    mToDisplay = to_display;
}