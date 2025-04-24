/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** CRM_Commands
*/

#ifndef CRM_COMMANDS_HPP_
#define CRM_COMMANDS_HPP_

#include "interfaces/IServerCommands.hpp"

class CRM_Commands : public IServerCommands {
    public:
        CRM_Commands() {};
        void execute(int id, std::string command, std::vector<NetworkSalon *> *mRooms, bool debugMode) override {
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);
            std::stringstream messageStream(command);
            std::vector<std::string> parts;
            std::string m;

            while (std::getline(messageStream, m, ' ')) {
                parts.push_back(m);
            }
            if (parts.size() != 3) {
                if (debugMode)
                    std::cerr << "[DEBUG] CRM Invalid command format" << std::endl;
                return;
            }
            std::string name = parts[1];
            int max_players = std::atoi(parts[2].c_str());

            try {
                mRooms->push_back(new NetworkSalon(name, max_players, debugMode));
            } catch (...) {
                if (debugMode)
                    std::cout << "[DEBUG] Error room sended an error" << std::endl;
                return;
            }
            
            if (debugMode)
                std::cout << "[DEBUG] Room " << name << " with a maxiumum players of " << max_players << "was created." << std::endl;
        };
};

#endif /* !CRM_COMMANDS_HPP_ */
