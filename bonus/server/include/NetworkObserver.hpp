/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** NetworkObserver
*/

#ifndef NETWORKOBSERVER_HPP_
#define NETWORKOBSERVER_HPP_

#include <iostream>
#include <unistd.h>
#include "interfaces/INetworkObserver.hpp"
#include "interfaces/INetwork.hpp"
#include "NetworkSalon.hpp"

class NetworkObserver : public INetworkObserver {
    public:
        NetworkObserver(NetworkSalon &salon, int PlayerSocket, bool debugMode);
        virtual ~NetworkObserver();
        void Update(const Message &message) override;
        void RemoveMe();
        NetworkSalon *getNetworkSalon();
    private:
        int mSocket;
        bool mDebugMode;
        static int mNumber;
        Message mMessageFromSubject;
        NetworkSalon &mNetworkSalon;
};


#endif /* !NETWORKOBSERVER_HPP_ */
