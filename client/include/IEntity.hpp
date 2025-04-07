/*
** EPITECH PROJECT, 2025
** _
** File description:
** _
*/

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class IEntity {
    public:
        virtual ~IEntity() = default;
    
        virtual void setPosition(std::pair<float, float> pos) = 0;
        virtual std::pair<float, float> getPosition() const = 0;
        virtual void updateAnimation() = 0;
        virtual sf::Sprite getSprite() const = 0;
};

#endif // ENTITY_HPP