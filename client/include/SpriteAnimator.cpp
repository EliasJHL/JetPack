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
        // mCurrentFrame = 0; // Reset to the first frame of the action
        mDefaultMode = false;
        mOneActionMode = false; // Ensure one-action mode is disabled
        updateTextureRect();
    }
}

void SpriteAnimator::setDefaultAction(int action) {
    if (action >= 0 && action < mActions) {
        mCurrentAction = action;
    }
    mDefaultMode = true;
    mOneActionMode = false; // Ensure one-action mode is disabled
    // mCurrentFrame = 0; // Start from the first frame
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
        // Default mode: Loop through all frames in the current row (0 to last frame)
        mCurrentFrame = (mCurrentFrame + 1) % mFramesPerAction;
    } else if (mOneActionMode) {
        // One-action mode: Stay on the same frame
        // Do nothing, as we are looping a single frame
    } else {
        // Normal mode: Loop through frames of the current action (1 to last frame, excluding 0)
        mCurrentFrame = (mCurrentFrame + 1) % mFramesPerAction;
        if (mCurrentFrame == 0) {
            mCurrentFrame = 1; // Skip frame 0
        }
    }

    std::cout << "Switching to frame " << mCurrentFrame << " of action " << mCurrentAction << std::endl;
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
    int left = mCurrentFrame * mFrameWidth; // Calculate the X position of the frame
    int top = mCurrentAction * mFrameHeight; // Calculate the Y position of the action
    mSprite.setTextureRect(sf::IntRect(left, top, mFrameWidth, mFrameHeight));
}
