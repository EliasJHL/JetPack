#include "ElectricBarrier.hpp"

ElectricBarrier::ElectricBarrier() : mAnimator(mSprite, 105.25, 122, 1), mAnimationTimer(0.3f) {
    if (!mTexture.loadFromFile("./client/ressources/Sprites/zapper_sprite_sheet.png")) {
        throw std::runtime_error("Failed to load electric barrier texture");
    }
    mSprite.setTexture(mTexture);
    mSprite.setScale(1.0, 1.0);
    mAnimator.setFramesPerAction(4);
}

void ElectricBarrier::setPosition(std::pair<float, float> pos) {
    mPos.x = pos.first;
    mPos.y = pos.second;
    mSprite.setPosition(mPos);
}

std::pair<float, float> ElectricBarrier::getPosition() const {
    return {mPos.x, mPos.y};
}

void ElectricBarrier::updateAnimation() {
    // Logic for animating the barrier (if needed)
}

sf::Sprite ElectricBarrier::getSprite() const {
    return mSprite;
}