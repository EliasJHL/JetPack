/*
** Player.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 11:43:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Mar 25 23:34:27 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "Player.hpp"

Player::Player(int id, const std::string &name, int socket)
    : mPlayerID(id), mPlayerName(name), mPlayerSocket(socket),
      mCoordinates({0, 0}), mConnected(true), mObserver(nullptr)
{
}

Player::~Player() {
    if (mObserver) {
        mObserver->RemoveMe();
        delete mObserver;
    }
}

unsigned int Player::getID(void) const
{
    return mPlayerID;
}

const std::string &Player::getName(void) const
{
    return mPlayerName;
}

bool Player::isConnected(void) const
{
    return mConnected;
}

const std::pair<float, float> &Player::getPosition(void) const
{
    return mCoordinates;
}

void Player::setPosition(std::pair<float, float> pos)
{
    mCoordinates = pos;
}

void Player::setConnection(bool connexion)
{
    mConnected = connexion;
}

NetworkObserver *Player::getObserver()
{
    return mObserver;
}

NetworkSalon *Player::getSalon()
{
    if (mObserver) {
        return mObserver->getNetworkSalon();
    }
    return nullptr;
}

void Player::setSalon(NetworkSalon &salon) {
    if (mObserver != nullptr) {
        mObserver->RemoveMe();
        delete mObserver;
    }
    mObserver = new NetworkObserver(salon);
}

int Player::getPlayerSocket() {
    return mPlayerSocket;
}