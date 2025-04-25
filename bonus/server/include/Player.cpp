/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** Player
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

void Player::addCoins(int nb, std::pair<float, float> coin, bool debugMode) {
    if (std::find(mCollectedCoins.begin(), mCollectedCoins.end(), coin) != mCollectedCoins.end())
        return;
    mPlayerCoin += nb;
    mCollectedCoins.push_back(coin);
    if (debugMode)
        std::cout << "[DEBUG] Player " << mPlayerID << " collected a coin !" << std::endl;
}

void Player::clearCollectedCoins()
{
    mCollectedCoins.clear();
}

const std::string &Player::getName(void) const
{
    return mPlayerName;
}

bool Player::isConnected(void) const
{
    return mConnected;
}

void Player::playerDie()
{
    mDied = true;
}

void Player::playerWin()
{
    mWin = true;
}

bool Player::isPlayerDied()
{
    return mDied;
}

bool Player::isPlayerWin()
{
    return mWin;
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

void Player::setSalon(NetworkSalon &salon, bool debugMode) {
    if (mObserver != nullptr) {
        mObserver->RemoveMe();
        delete mObserver;
    }
    mObserver = new NetworkObserver(salon, mPlayerSocket, debugMode);
}

int Player::getPlayerSocket() {
    return mPlayerSocket;
}