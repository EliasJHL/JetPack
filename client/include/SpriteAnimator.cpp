/*
** EPITECH PROJECT, 2025
** Game
** File description:
** SpriteAnimator
*/

#include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator(sf::Sprite &sprite, int frameWidth, int frameHeight, int actions)
    : mSprite(sprite), mFrameWidth(frameWidth), mFrameHeight(frameHeight), mActions(actions), mCurrentFrame(0), mCurrentAction(0), mFramesPerAction(4)
{
}

void SpriteAnimator::setAction(int action)
{
    if (action >= 0 && action < mActions) {
        mCurrentAction = action;
        mCurrentFrame = 0;
        mDefaultMode = false;
        mOneActionMode = false;
        updateTextureRect();
    }
}

void SpriteAnimator::setDefaultAction()
{
    mDefaultMode = true;
    mOneActionMode = false;
    mCurrentFrame = 0;
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

void SpriteAnimator::nextFrame()
{
    if (mDefaultMode) {
        if (mCurrentFrame < mFramesPerAction - 1) {
            mCurrentFrame++;
        } else {
            // End of the row
            if (mCurrentAction < mActions - 1) {
                mCurrentAction++;
                mCurrentFrame = 0;
            } else {
                mCurrentAction = 0;
                mCurrentFrame = 1;
            }
        }
    } else if (mOneActionMode) {
    } else {
        mCurrentFrame = (mCurrentFrame + 1) % mFramesPerAction;
    }
    updateTextureRect();
}

void SpriteAnimator::setFramesPerAction(int frames)
{
    mFramesPerAction = frames;
}

sf::IntRect SpriteAnimator::getTextureRect() const
{
    return sf::IntRect(mCurrentFrame * mFrameWidth, mCurrentAction * mFrameHeight, mFrameWidth, mFrameHeight);
}
