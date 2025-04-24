/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** NetworkSalon
*/

#ifndef NETWORKSALON_HPP_
#define NETWORKSALON_HPP_

#include <iostream>
#include <unistd.h>
#include <list>
#include "interfaces/INetworkObserver.hpp"
#include "interfaces/INetwork.hpp"

class NetworkSalon : public INetwork {
    public:
        NetworkSalon(const std::string &name, int max_players, bool debugMode);
        virtual ~NetworkSalon() {};
        
        bool Join(INetworkObserver *obs) override;

        void Quit(INetworkObserver *obs) override;
        
        void Update() override;

        void CreateMessage(const std::string &message, Type type, int id);

        int HowManyPlayers();
        
        int HowMaxPlayers();

        std::string getSalonName();
    private:
        std::list<INetworkObserver *> mListObserver;
        Message mMessage;
        bool mDebugMode;
        int mMaxPlayers;
        std::string mSalonName;
};

#endif /* !NETWORKSALON_HPP_ */
