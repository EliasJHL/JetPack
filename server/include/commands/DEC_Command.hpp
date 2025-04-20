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
        void execute(int id, std::string command) override {
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);
            std::regex const e{"^DEC\\s+(\\d+)$"};
            std::smatch m;

            if (!std::regex_search(command, m, e))
                return;
            player->getSalon()->CreateMessage(std::string("DEC " + std::to_string(player->getID())), Type::DISCONNECT, player->getID());
            player->getSalon()->Quit(player->getObserver());
            close(player->getPlayerSocket());
            plyManager->removePlayer(player->getID());
            // for (auto it = mPoll.begin(); it != mPoll.end(); ++it) {
            //     if (it->fd == player->getPlayerSocket()) {
            //         mPoll.erase(it);
            //         close(player->getPlayerSocket());
            //         break;
            //     }
            // }
        };
};

#endif /* !DEC_COMMAND_HPP_ */
