/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** DEC_Command
*/

#ifndef DEC_COMMAND_HPP_
#define DEC_COMMAND_HPP_

#include "interfaces/IServerCommands.hpp"

class DEC_Command : public IServerCommands {
    public:
        DEC_Command() {};
        void execute(int id, std::string command, std::vector<NetworkSalon *> *mRooms, bool debugMode) override {
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);

            if (player->getSalon() != nullptr) {
                player->getSalon()->CreateMessage(std::string("DEC " + std::to_string(player->getID())), Type::DISCONNECT, player->getID());
                player->getSalon()->Quit(player->getObserver());
            }
            close(player->getPlayerSocket());
            //plyManager->removePlayer(player->getID());
            if (debugMode)
                std::cout << "[DEBUG] Player " << id << " disconnected by DEC command" << std::endl;
        };
};

#endif /* !DEC_COMMAND_HPP_ */
