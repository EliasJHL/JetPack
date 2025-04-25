/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Animations
*/

#include "GameManager.hpp"

void GameManager::move_background(void)
{
    Player* player = mPlayerManager->getPlayer(mPlayerID);
    std::pair<float, float> pos;

    if (player == nullptr)
        return;
    pos = player->getPosition();
    sf::Sprite backgroundSprite(mBackground);

    if (mViewPos.x > mMapWidth * mScaleFactor) {
        mViewPos.x = 0;
        player->setPosition({0, pos.second});
    }
    backgroundSprite.setScale(2.5f, 2.5f);
    backgroundSprite.setPosition(-4315, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(4315, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(0, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(8630, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(12945, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(17160, -70);
    mWindow.draw(backgroundSprite);
}

void GameManager::handleAnimations(void)
{
    if (!mHasUsername || !mGameReady)
        return;

    Player* player = mPlayerManager->getPlayer(mPlayerID);
    if (player == nullptr)
        return;
    std::pair<float, float> pos = player->getPosition();
    bool SpaceKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mWindow.hasFocus();
    bool ArrowLeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && mWindow.hasFocus();
    bool RightArrowPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && mWindow.hasFocus();
    bool IsOnGround = (pos.second >= FLOOR);
    bool IsPlayerDead = player->isDead();
    bool IsFlying = !IsOnGround && SpaceKeyPressed;
    bool IsWasFlying = !IsOnGround && !SpaceKeyPressed;

    if (SpaceKeyPressed && !IsPlayerDead) {
        if (IsOnGround) {
            if (!mSoundManager.isSoundPlaying("jump"))
                mSoundManager.playSound("jump");
        } else {
            if (!mSoundManager.isSoundPlaying("fly"))
                mSoundManager.playSound("fly", true);
        }
        if (pos.second > 0) {
            pos.second -= 5;
        }
        player->setAction(1, 2);
    } else if (pos.second < FLOOR) {
        pos.second += 5;
        if (!IsPlayerDead) {
            player->setAction(1, 1);
        } else
            player->setAction(3, 0);
    } else {
        if (!IsWasFlying) {
            mSoundManager.stopSound("fly");
            mSoundManager.playSound("stopfly");
        }
        if (!IsOnGround) {
            pos.second += 5;
            if (!IsPlayerDead)
                player->setAction(1, 1);
            else
                player->setAction(3, 0);
        } else {
            if (!IsPlayerDead)
                player->setAction(0, 0);
            else
                player->setAction(3, 0);
        }
    }

    player->updateScoreText(mViewPos);
    if (ArrowLeftPressed) {
        pos.first += 1;
    } else if (RightArrowPressed) {
        pos.first += 8;
    } else {
        pos.first += 4;
    }
    if (!player->isDead() && !player->isWin())
        mViewPos.x += 4;
    player->setPosition({pos.first, pos.second});
    player->updateAnimation();
    mView.setCenter(mViewPos.x, mView.getCenter().y);
    mWindow.setView(mView);

    // Update Other players animations
    for (Player *p : mPlayerManager->getAllPlayers()) {
        bool isLocalPlayer = (p->getID() == mPlayerID);
        if (isLocalPlayer)
            continue;
        pos = p->getPosition();
        IsOnGround = (pos.second >= FLOOR);
        IsPlayerDead = p->isDead();

        if (pos.second < FLOOR) {
            if (!IsPlayerDead)
                p->setAction(1, 2);
            else
                p->setAction(3, 0);
        } else {
            if (!IsOnGround) {
                if (!IsPlayerDead)
                    p->setAction(1, 1);
                else
                    p->setAction(3, 0);
            } else {
                if (!IsPlayerDead)
                    p->setAction(0, 0);
                else
                    p->setAction(3, 0);
            }
        }
    }

    // Update Animations
    for (Player *p : mPlayerManager->getAllPlayers()) {
        bool isLocalPlayer = (p->getID() == mPlayerID);
        if (!isLocalPlayer && p != nullptr)
            p->updateAnimation();
    }

    IsWasFlying = !IsOnGround && !SpaceKeyPressed;
}