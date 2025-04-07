#include "Coin.hpp"

Coin::Coin() : mAnimator(mSprite, 191.933, 171, 1), mAnimationTimer(0.15f) {
    if (!mTexture.loadFromFile("./client/ressources/sprites/coins_sprite_sheet.png")) {
        throw std::runtime_error("Failed to load coin spritesheet");
    }
    mSprite.setTexture(mTexture);
    mSprite.setScale(0.5, 0.5);
    mAnimator.setFramesPerAction(6); // 6 frames pour l'animation
    mAnimator.setDefaultAction(0);    // Commence sur la première frame
    mPos.x = 400;
    mPos.y = 450;
    mSprite.setPosition(mPos);
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
    if (mAnimationTimer.isElapsed()) { // Vérifie si l'intervalle est écoulé
        mAnimator.nextFrame();         // Passe à la frame suivante
        mAnimationTimer.restart();     // Redémarre le Timer
    }
    mSprite.setTextureRect(mAnimator.getTextureRect()); // Met à jour la texture
}

sf::Sprite Coin::getSprite() const {
    return mSprite;
}