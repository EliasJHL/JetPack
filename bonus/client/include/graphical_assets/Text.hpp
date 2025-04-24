/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Text
*/

#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "Color.hpp"
#include <string>

class Text {
    public:
        Text(int x, int y, int size, const std::string &text, bool centeredX, bool centeredY, Color color)
            : _x(x), _y(y), _size(size), _text(text), _centeredX(centeredX), _centeredY(centeredY), _color(color) {}
        ~Text() = default;

        int getX() const { return _x; };
        int getY() const { return _y; };
        int getSize() const { return _size; };
        std::string getFont() const { return _font; };
        std::string getText() const { return _text; };
        Color getColor() const { return _color; };
        bool isCenteredX() const { return _centeredX; };
        bool isCenteredY() const { return _centeredY; };

    private:
        int _x;
        int _y;
        bool _centeredX;
        bool _centeredY;
        int _size;
        std::string _font;
        std::string _text;
        Color _color;
};

#endif /* !TEXT_HPP_ */
