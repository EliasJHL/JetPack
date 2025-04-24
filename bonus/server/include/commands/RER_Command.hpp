/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** RER_Commands
*/

#ifndef RER_COMMANDS_HPP_
#define RER_COMMANDS_HPP_

#include "interfaces/IServerCommands.hpp"

class RER_Commands : public IServerCommands {
    public:
        RER_Commands() {};
        void execute(int id, std::string command, std::vector<NetworkSalon *> *mRooms, bool debugMode) override {
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);
            
            if (debugMode)
                std::cout << "[DEBUG] Player " << id << " requested online rooms " << std::endl;

            for (NetworkSalon *s : *mRooms) {
                std::string m = "ROM " + s->getSalonName() + " " + std::to_string(s->HowMaxPlayers()) + " " + std::to_string(s->HowManyPlayers()) + "\r\n";
                write(player->getPlayerSocket(), m.c_str(), m.length());
            }
            if (debugMode)
                std::cout << "[DEBUG] For Player " << id << " all rooms were sended " << std::endl;
        };
};

#endif /* !RER_COMMANDS_HPP_ */
