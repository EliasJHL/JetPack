/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** POS_Command
*/

#ifndef POS_COMMAND_HPP_
#define POS_COMMAND_HPP_

#include "interfaces/IServerCommands.hpp"

class POS_Command : public IServerCommands {
    public:
        POS_Command() {};
        void execute(int id, std::string command) override {
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);
            std::stringstream messageStream(command);
            std::vector<std::string> parts;
            std::string m;

            while (std::getline(messageStream, m, ' ')) {
                parts.push_back(m);
            }
            player->setPosition(std::pair<float, float> {std::stof(parts[1].c_str()), std::stof(parts[2].c_str())});
        };
};

#endif /* !POS_COMMAND_HPP_ */
