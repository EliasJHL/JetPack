/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** SNA_Command
*/

#ifndef SNA_COMMAND_HPP_
#define SNA_COMMAND_HPP_

#include "interfaces/IServerCommands.hpp"

class SNA_Command : public IServerCommands {
    public:
        SNA_Command() {};
        void execute(int id, std::string command) override {
            std::smatch m;
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);
            std::regex const e{"^SNA\\s+([A-Za-z0-9]+)$"};

            if (!std::regex_search(command, m, e))
                return;

            player->setPlayerName(m[1]);
            
            // Envoyer au nouveau joueur les joueurs déjà connectés
            std::string existingPlayer = "JON " + std::to_string(player->getID()) + " " + player->getName() + "\r\n";
            for (Player* p : plyManager->getAllPlayers()) {
                if (p != nullptr && p->getName() != "Dummy" && p->getID() != player->getID()) {
                    std::string existingPlayer = "JON " + std::to_string(p->getID()) + " " + p->getName() + "\r\n";
                    write(player->getPlayerSocket(), existingPlayer.c_str(), existingPlayer.length());
                }
            }
            
            // Envoyer à tous les joueurs du nouveau joueur
            std::string newPlayer = "JON " + std::to_string(player->getID()) + " " + m[1].str() + "\r\n";
            player->getSalon()->CreateMessage(newPlayer, Type::CONNECT, player->getID());
            
            // Lancer le jeu si +2 joueurs sont connectés
            std::string srt_msg = "SRT\r\n";
            if (plyManager->getReadyPlayer().size() >= 2) {
                std::cout << "OK 2 Players online" << std::endl;
                player->getSalon()->CreateMessage(srt_msg, Type::START, player->getID());
            }
        };
};

#endif /* !SNA_COMMAND_HPP_ */
