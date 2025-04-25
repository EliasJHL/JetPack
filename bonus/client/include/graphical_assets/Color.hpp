/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Color
*/

#ifndef COLOR_HPP_
#define COLOR_HPP_

class Color {
    public:
        Color(int r, int g, int b, int a) {
            _r = r;
            _g = g;
            _b = b;
            _a = a;
        };
        Color() {
            _r = 255;
            _g = 255;
            _b = 255;
            _a = 255;
        };
        ~Color() = default;
        int getR() const {
            return _r;
        };
        int getG() const {
            return _g;
        };
        int getB() const {
            return _b;
        };
        int getA() const {
            return _a;
        };
    private:
        int _r;
        int _g;
        int _b;
        int _a;
};

#endif /* !COLOR_HPP_ */
