/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** Factory
*/

#ifndef FACTORY_HPP_
#define FACTORY_HPP_
#include "Map.hpp"
#include "Player.hpp"
#include <map>
#include <string>
#include "../interfaces/ICommands.hpp"

class Factory {
    public:
        Factory();
        ~Factory();
        ICommands *getCommand(std::string cmd);
    private:
        std::map<std::string, ICommands*> commands;
};

#endif /* !FACTORY_HPP_ */
