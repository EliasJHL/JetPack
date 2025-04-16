#ifndef ELECTRICBARRIER_HPP_
#define ELECTRICBARRIER_HPP_

#include "interfaces/IEntity.hpp"
#include "Clock.hpp"
#include "SpriteAnimator.hpp"

class ElectricBarrier : public IEntity {
public:
    ElectricBarrier();
    ~ElectricBarrier() = default;

    void setPosition(std::pair<float, float> pos) override;
    std::pair<float, float> getPosition() const override;
    void updateAnimation() override;
    sf::Sprite &getSprite() override;

private:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPos;
    SpriteAnimator mAnimator;
    Clock mAnimationTimer; // Utilisation de la classe Timer
};

#endif /* ELECTRICBARRIER_HPP_ */