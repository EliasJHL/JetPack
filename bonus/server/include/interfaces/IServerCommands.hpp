/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** IServerCommands
*/

#ifndef ISERVERCOMMANDS_HPP_
#define ISERVERCOMMANDS_HPP_

#include <string>
#include "server.hpp"
#include <regex>
#include "INetwork.hpp"
#include "NetworkSalon.hpp"

class IServerCommands {
    public:
        virtual ~IServerCommands() = default;
        virtual void execute(int id, std::string command, std::vector<NetworkSalon *> *mRooms, bool debugMode) = 0;
};

#endif /* !ISERVERCOMMANDS_HPP_ */
