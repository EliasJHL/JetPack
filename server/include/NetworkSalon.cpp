/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** NetworkSalon
*/

#include "NetworkSalon.hpp"

NetworkSalon::NetworkSalon(const std::string &name, bool debugMode) : mSalonName(name)
{
    mDebugMode = debugMode;
}

void NetworkSalon::Join(INetworkObserver *obs)
{
    if (mDebugMode)
        std::cout << "[DEBUG] " << mSalonName << " : New player joined the salon" << std::endl;
    mListObserver.push_back(obs);
};

void NetworkSalon::Quit(INetworkObserver *obs)
{
    mListObserver.remove(obs);
};

void NetworkSalon::Update()
{
    std::list<INetworkObserver *>::iterator it = mListObserver.begin();
    while (it != mListObserver.end()) {
        if (*it != nullptr) {
            (*it)->Update(mMessage);
        }
        ++it;
    }
};

void NetworkSalon::CreateMessage(const std::string &message, Type type, int id)
{
    mMessage.message = message;
    mMessage.PlayerID = id;
    mMessage.type = type;
    Update();
};

int NetworkSalon::HowManyPlayers()
{
    return mListObserver.size();
}

std::string NetworkSalon::getSalonName()
{
    return mSalonName;
}