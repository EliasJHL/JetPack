#ifndef COIN_HPP_
#define COIN_HPP_

#include "interfaces/IEntity.hpp"
#include "Clock.hpp"
#include "SpriteAnimator.hpp"

class Coin : public IEntity {
public:
    Coin();
    ~Coin() = default;

    void setPosition(std::pair<float, float> pos) override;
    std::pair<float, float> getPosition() const override;
    void updateAnimation() override;
    sf::Sprite &getSprite() override;
    void toDisplay(bool to_display);
    bool isVisible() const;

private:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPos;
    SpriteAnimator mAnimator;
    Clock mAnimationTimer;
    bool mToDisplay;
};

#endif /* COIN_HPP_ */