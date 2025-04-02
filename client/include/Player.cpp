/*
** Player.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:51:49 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 23:30:25 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "Player.hpp"

Player::Player(int id, std::string name)
{
    mPlayerID = id;
    mPlayerName = name;
    mTexture.loadFromFile("./client/ressources/Sprites/b.jpg");
    mSprite.setTexture(mTexture);
    mSprite.setScale(0.05, 0.05);
    mPos.x = 350;
    mPos.y = 570;
    mSprite.setPosition(mPos);
}

Player::~Player()
{
    
}

int Player::getID(void) const
{
    return mPlayerID;
}

std::string Player::getName(void) const
{
    return mPlayerName;
}

std::pair<float, float> Player::getPosition(void) const
{
    return {mPos.x, mPos.y};
}

void Player::setPosition(std::pair<float, float> pos)
{
    mPos.x = pos.first;
    mPos.y = pos.second;
    if (mPos.y > 570)
        mPos.y = 570;
    if (mPos.y < 0)
        mPos.y = 0;
    mSprite.setPosition(mPos);
}

sf::Sprite Player::getPlayerSprite(void)
{
    mSprite.setPosition(mPos);
    return mSprite;
}