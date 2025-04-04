#include "Coin.hpp"

Coin::Coin() {
    if (!mTexture.loadFromFile("./client/ressources/Sprites/coin_sprite_sheet.png")) {
        throw std::runtime_error("Failed to load coin spritesheet");
    }
    mSprite.setTexture(mTexture);
    mSprite.setScale(0.5, 0.5);
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
    // Logic for animating the coin (if needed)
}

sf::Sprite Coin::getSprite() const {
    return mSprite;
}