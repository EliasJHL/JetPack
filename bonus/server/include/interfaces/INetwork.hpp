/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** INetwork
*/

#ifndef INETWORK_HPP_
#define INETWORK_HPP_

#include "interfaces/INetworkObserver.hpp"

class INetwork {
    public:
        virtual ~INetwork(){};
        virtual bool Join(INetworkObserver *obs) = 0;
        virtual void Quit(INetworkObserver *obs) = 0;
        virtual void Update() = 0;
};

#endif /* !INETWORK_HPP_ */
