/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Rect
*/

#ifndef RECT_HPP_
# define RECT_HPP_

#include <string>
#include "Color.hpp"

class Rect {
    public:
        Rect(int x, int y, int width, int height, Color color) {
            _x = x;
            _y = y;
            _width = width;
            _height = height;
            _color = color;
        };
        ~Rect() = default;
        int getX() const {
            return _x;
        };
        int getY() const {
            return _y;
        };
        int getWidth() const {
            return _width;
        };
        int getHeight() const {
            return _height;
        };
        Color getColor() const {
            return _color;
        };
    private:
        int _x;
        int _y;
        int _width;
        int _height;
        Color _color;
};

#endif /* !RECT_HPP_ */