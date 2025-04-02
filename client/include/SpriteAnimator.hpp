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
    int mFrameWidth; // Width of a single frame
    int mFrameHeight; // Height of a single frame
    int mActions; // Number of actions (rows in the spritesheet)
    int mFramesPerAction; // Number of frames per action (columns in the spritesheet)
    int mCurrentFrame; // Current frame index (column)
    int mCurrentAction; // Current action index (row)
    bool mDefaultMode = false; // Whether the animator is in default mode
    bool mOneActionMode = false; // Whether the animator is in one-action mode
};

#endif /* SPRITEANIMATOR_HPP_ */