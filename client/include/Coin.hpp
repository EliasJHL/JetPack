#ifndef COIN_HPP_
#define COIN_HPP_

#include "IEntity.hpp"
#include "Clock.hpp"
#include "SpriteAnimator.hpp"

class Coin : public IEntity {
public:
    Coin();
    ~Coin() = default;

    void setPosition(std::pair<float, float> pos) override;
    std::pair<float, float> getPosition() const override;
    void updateAnimation() override;
    sf::Sprite getSprite() const override;

private:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPos;
    SpriteAnimator mAnimator;
    Clock mAnimationTimer; // Utilisation de la classe Timer
};

#endif /* COIN_HPP_ */