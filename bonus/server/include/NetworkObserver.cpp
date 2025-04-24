/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** NetworkObserver
*/

#include "NetworkObserver.hpp"

int NetworkObserver::mNumber = 0;

NetworkObserver::NetworkObserver(NetworkSalon &salon, int PlayerSocket, bool debugMode) : mNetworkSalon(salon) 
{
    mNumber = mNumber++;
    mSocket = PlayerSocket;
    mDebugMode = debugMode;
    if (mDebugMode)
        std::cout << "[DEBUG] Welcome into salon " << salon.getSalonName() << " \n";
}

NetworkObserver::~NetworkObserver()
{
    if (mDebugMode)
        std::cout << "[DEBUG] Goodbye, I was the Observer \"" << this->mNumber << "\".\n";
}

void NetworkObserver::Update(const Message &message)
{
    mMessageFromSubject = message;
    write(mSocket, std::string(message.message + "\r\n").c_str(), message.message.length() + 2);
};

void NetworkObserver::RemoveMe()
{
    mNetworkSalon.Quit(this);
};

NetworkSalon *NetworkObserver::getNetworkSalon()
{
    return &mNetworkSalon;
}