/*
** EPITECH PROJECT, 2025
** Game
** File description:
** SpriteAnimator
*/

#ifndef SPRITEANIMATOR_HPP_
#define SPRITEANIMATOR_HPP_

#include <SFML/Graphics.hpp>

class SpriteAnimator {
public:
    SpriteAnimator(sf::Sprite &sprite, int frameWidth, int frameHeight, int actions);
    ~SpriteAnimator() = default;

    void setAction(int action);

    void setDefaultAction(int action);

    void setOneAction(int action, int frame);

    void nextFrame();

    void setFramesPerAction(int frames);

    sf::IntRect getTextureRect() const;

private:
    void updateTextureRect();
    sf::Sprite &mSprite;
    int mFrameWidth;
    int mFrameHeight;
    int mActions;
    int mFramesPerAction;
    int mCurrentFrame;
    int mCurrentAction;
    bool mDefaultMode = false;
    bool mOneActionMode = false;
};

#endif /* SPRITEANIMATOR_HPP_ */