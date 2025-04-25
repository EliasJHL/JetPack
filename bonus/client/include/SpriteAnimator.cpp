/*
** EPITECH PROJECT, 2025
** Game
** File description:
** SpriteAnimator
*/

#include "SpriteAnimator.hpp"
#include <iostream>


SpriteAnimator::SpriteAnimator(sf::Sprite &sprite, int frameWidth, int frameHeight, int actions)
    : mSprite(sprite), mFrameWidth(frameWidth), mFrameHeight(frameHeight), mActions(actions), mCurrentFrame(0), mCurrentAction(0), mFramesPerAction(4)
{
}

void SpriteAnimator::setAction(int action) {
    if (action >= 0 && action < mActions) {
        mCurrentAction = action;
        mDefaultMode = false;
        mOneActionMode = false;
        updateTextureRect();
    }
}

void SpriteAnimator::setDefaultAction(int action) {
    if (action >= 0 && action < mActions) {
        mCurrentAction = action;
    }
    mDefaultMode = true;
    mOneActionMode = false;
    updateTextureRect();
}

void SpriteAnimator::setOneAction(int action, int frame)
{
    if (action >= 0 && action < mActions && frame >= 0 && frame < mFramesPerAction) {
        mOneActionMode = true;
        mDefaultMode = false;
        mCurrentAction = action;
        mCurrentFrame = frame;
        updateTextureRect();
    }
}

void SpriteAnimator::nextFrame() {
    if (mDefaultMode) {
        mCurrentFrame = (mCurrentFrame + 1) % mFramesPerAction;
    } else if (mOneActionMode) {
    } else {
        mCurrentFrame = (mCurrentFrame + 1) % mFramesPerAction;
        if (mCurrentFrame == 0) {
            mCurrentFrame = 1;
        }
    }
    updateTextureRect();
}

void SpriteAnimator::setFramesPerAction(int frames) {
    if (frames > 0) {
        mFramesPerAction = frames;
    } else {
        throw std::runtime_error("Frames per action must be greater than 0");
    }
}

sf::IntRect SpriteAnimator::getTextureRect() const
{
    return sf::IntRect(mCurrentFrame * mFrameWidth, mCurrentAction * mFrameHeight, mFrameWidth, mFrameHeight);
}

void SpriteAnimator::updateTextureRect() {
    int left = mCurrentFrame * mFrameWidth;
    int top = mCurrentAction * mFrameHeight;
    mSprite.setTextureRect(sf::IntRect(left, top, mFrameWidth, mFrameHeight));
}
