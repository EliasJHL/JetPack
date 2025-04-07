#ifndef ELECTRICBARRIER_HPP_
#define ELECTRICBARRIER_HPP_

#include "IEntity.hpp"

class ElectricBarrier : public IEntity {
public:
    ElectricBarrier();
    ~ElectricBarrier() = default;

    void setPosition(std::pair<float, float> pos) override;
    std::pair<float, float> getPosition() const override;
    void updateAnimation() override;
    sf::Sprite getSprite() const override;

private:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPos;
};

#endif /* ELECTRICBARRIER_HPP_ */