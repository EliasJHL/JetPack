/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** Factory
*/

#ifndef FACTORY_HPP_
#define FACTORY_HPP_
#include "interfaces/IServerCommands.hpp"
#include <map>
#include <string>

#include "commands/DEC_Command.hpp"
#include "commands/SNA_Command.hpp"
#include "commands/POS_Command.hpp"


class Factory {
    public:
        Factory() {
            commands["SNA"] = new SNA_Command();
            commands["DEC"] = new DEC_Command();
            commands["POS"] = new POS_Command();
        };
        ~Factory() {
            for (auto& pair : commands) {
                delete pair.second;
            }
            commands.clear();
        };
        IServerCommands *getCommand(std::string cmd) {
            auto it = commands.find(cmd);
            if (it != commands.end()) {
                return it->second;
            }
            return nullptr;
        };
    private:
        std::map<std::string, IServerCommands*> commands;
};

#endif /* !FACTORY_HPP_ */
