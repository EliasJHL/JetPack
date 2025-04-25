/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** TextSector
*/

#ifndef TEXTSECTOR_HPP_
#define TEXTSECTOR_HPP_

#include "Text.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class TextSector : public Text {
    public:
        TextSector(int x, int y, int width, int height, int size, const std::string &text, 
                  bool centeredX, bool centeredY, Color color, Color backgroundColor = Color(0, 0, 0, 0))
            : Text(x, y, size, text, centeredX, centeredY, color), 
              _width(width), _height(height), _backgroundColor(backgroundColor), _isActive(false) {}
        
        ~TextSector() = default;

        int getWidth() const { return _width; }
        int getHeight() const { return _height; }
        Color getBackgroundColor() const { return _backgroundColor; }
        bool isActive() const { return _isActive; }
        
        void setWidth(int width) { _width = width; }
        void setHeight(int height) { _height = height; }
        void setBackgroundColor(Color color) { _backgroundColor = color; }
        void setActive(bool active) { _isActive = active; }
        
        bool contains(int x, int y) const {
            int sectorX = getX();
            int sectorY = getY();
            
            if (isCenteredX())
                sectorX -= _width / 2;
            if (isCenteredY())
                sectorY -= _height / 2;
            
            return (x >= sectorX && x <= sectorX + _width &&
                    y >= sectorY && y <= sectorY + _height);
        }
        
        void setOnClickCallback(std::function<void()> callback) {
            _onClick = callback;
        }
        
        void click() const {
            if (_onClick) {
                _onClick();
            }
        }
        
        void draw(sf::RenderWindow& window, sf::Font& font) const {
            sf::RectangleShape background;
            int sectorX = getX();
            int sectorY = getY();
            
            if (isCenteredX()) {
                sectorX -= _width / 2;
            }
            if (isCenteredY()) {
                sectorY -= _height / 2;
            }
            
            background.setPosition(sectorX, sectorY);
            background.setSize(sf::Vector2f(_width, _height));
            background.setFillColor(sf::Color(
                _backgroundColor.getR(),
                _backgroundColor.getG(),
                _backgroundColor.getB(),
                _backgroundColor.getA()
            ));
            
            if (_isActive) {
                background.setOutlineThickness(2);
                background.setOutlineColor(sf::Color(255, 255, 255, 128));
            }
            
            window.draw(background);
            
            sf::Text sfText;
            sfText.setFont(font);
            sfText.setString(getText());
            sfText.setCharacterSize(getSize());
            sfText.setFillColor(sf::Color(
                getColor().getR(),
                getColor().getG(),
                getColor().getB(),
                getColor().getA()
            ));
            
            float textX = getX();
            float textY = getY();
            
            if (isCenteredX()) {
                textX -= sfText.getGlobalBounds().width / 2;
            }
            if (isCenteredY()) {
                textY -= sfText.getGlobalBounds().height / 2;
            }
            
            sfText.setPosition(textX, textY);
            window.draw(sfText);
        }

    private:
        int _width;
        int _height;
        Color _backgroundColor;
        bool _isActive;
        std::function<void()> _onClick = nullptr;
};

#endif /* !TEXTSECTOR_HPP_ */
