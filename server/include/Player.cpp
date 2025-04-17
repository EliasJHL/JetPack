/*
** Player.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 11:43:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:02:02 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "Player.hpp"

Player::Player(int id, const std::string &name, int socket)
    : mPlayerID(id), mPlayerName(name), mPlayerSocket(socket),
      mCoordinates({0, 0}), mConnected(true), mObserver(nullptr), mPlayerCoin(0), mToDelete(false)
{
}

Player::~Player() {
    if (mObserver) {
        mObserver->RemoveMe();
        delete mObserver;
    }
}

int Player::getID(void) const
{
    return mPlayerID;
}

int Player::getCoins()
{
    return mPlayerCoin;
}

void Player::addCoins(int nb)
{
    mPlayerCoin += nb;
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

void Player::setPlayerName(const std::string &name)
{
    mPlayerName = name;
}

NetworkObserver *Player::getObserver()
{
    return mObserver;
}

NetworkSalon *Player::getSalon()
{
    std::lock_guard<std::mutex> lock(mAccessMutex);
    if (mToDelete || mObserver == nullptr)
        return nullptr;
    return mObserver->getNetworkSalon();
}

void Player::setSalon(NetworkSalon &salon) {
    if (mObserver != nullptr) {
        mObserver->RemoveMe();
        delete mObserver;
    }
    mObserver = new NetworkObserver(salon, mPlayerSocket);
}

int Player::getPlayerSocket() {
    return mPlayerSocket;
}