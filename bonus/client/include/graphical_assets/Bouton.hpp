/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Bouton
*/

#ifndef BOUTON_HPP_
#define BOUTON_HPP_

#include "Color.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include "GameManager.hpp"

class Bouton {
    public:
        Bouton(int x, int y, int size, const std::string &text, bool centeredX, bool centeredY, Color color, std::string salon="")
            : _x(x), _y(y), _size(size), _text(text), _centeredX(centeredX), _centeredY(centeredY), _color(color),
             _salon_name(salon) {}
        Bouton() {};
        ~Bouton() = default;
        int getX() const { return _x; };
        int getY() const { return _y; };
        int getSize() const { return _size; };
        std::string getText() const { return _text; };
        Color getColor() const { return _color; };
        bool isCenteredX() const { return _centeredX; };
        bool isCenteredY() const { return _centeredY; };
        bool isClicked(int mouseX, int mouseY, sf::Font &font) const
        {
            sf::Text sfText;
            sfText.setFont(font);
            sfText.setString(_text);
            sfText.setCharacterSize(_size);

            float posX = _x;
            float posY = _y;
            if (_centeredX)
                posX -= sfText.getGlobalBounds().width / 2;
            if (_centeredY)
                posY -= sfText.getGlobalBounds().height / 2;

            sfText.setPosition(posX, posY);
            
            sf::FloatRect bounds = sfText.getGlobalBounds();
            bounds.height = bounds.height * 1.5;
            return bounds.contains(mouseX, mouseY);
        }


    private:
        int _x;
        int _y;
        bool _centeredX;
        bool _centeredY;
        int _size;
        std::string _text;
        std::string _salon_name;
        Color _color;
};

#endif /* !BOUTON_HPP_ */
