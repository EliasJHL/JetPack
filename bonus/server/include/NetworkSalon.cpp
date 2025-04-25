/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** NetworkSalon
*/

#include "NetworkSalon.hpp"

NetworkSalon::NetworkSalon(const std::string &name, int max_players, bool debugMode) : mSalonName(name), mMaxPlayers(max_players)
{
    mDebugMode = debugMode;
}

bool NetworkSalon::Join(INetworkObserver *obs)
{
    if (HowManyPlayers() >= mMaxPlayers)
        return false;
    if (mDebugMode)
        std::cout << "[DEBUG] " << mSalonName << " : New player joined the salon" << std::endl;
    mListObserver.push_back(obs);
    return true;
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

int NetworkSalon::HowMaxPlayers()
{
    return mMaxPlayers;
}

std::string NetworkSalon::getSalonName()
{
    return mSalonName;
}