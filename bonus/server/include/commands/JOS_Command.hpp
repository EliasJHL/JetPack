/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** JOS_Commands
*/

#ifndef JOS_COMMANDS_HPP_
#define JOS_COMMANDS_HPP_

#include "interfaces/IServerCommands.hpp"

class JOS_Commands : public IServerCommands {
    public:
        JOS_Commands() {};
        void execute(int id, std::string command, std::vector<NetworkSalon *> *mRooms, bool debugMode) override {
            PlayersManager *plyManager = PlayersManager::getInstance();
            Player *player = plyManager->getPlayer(id);
            std::stringstream messageStream(command);
            std::vector<std::string> parts;
            std::string m;

            while (std::getline(messageStream, m, ' ')) {
                parts.push_back(m);
            }
            if (parts.size() != 2) {
                if (debugMode)
                    std::cerr << "Invalid command format" << std::endl;
                return;
            }
            std::string salonName = parts[1];
            if (player->getSalon() != nullptr) {
                player->getSalon()->Quit(player->getObserver());
            }
            NetworkSalon *salon = nullptr;
            for (auto &room : *mRooms) {
                if (room->getSalonName() == salonName) {
                    salon = room;
                    break;
                }
            }
            if (salon == nullptr) {
                if (debugMode)
                    std::cerr << "Salon with name '" << salonName << "' not found." << std::endl;
                return;
            }

            player->setSalon(*salon, debugMode);
            std::string message;
            if (salon->Join(player->getObserver())) {
                message = "JOK\r\n";
                write(player->getPlayerSocket(), message.c_str(), message.length());
            } else {
                message = "JNO\r\n";
                write(player->getPlayerSocket(), message.c_str(), message.length());
                return;
            }            

            // Envoyer au nouveau joueur les joueurs déjà connectés
            std::string existingPlayer = "JON " + std::to_string(player->getID()) + " " + player->getName() + "\r\n";
            for (Player* p : plyManager->getAllPlayers()) {
                if (p != nullptr && p->getName() != "Dummy" && p->getID() != player->getID() && p->getSalon() == player->getSalon()) {
                    std::string existingPlayer = "JON " + std::to_string(p->getID()) + " " + p->getName() + "\r\n";
                    write(player->getPlayerSocket(), existingPlayer.c_str(), existingPlayer.length());
                }
            }
            
            // Envoyer à tous les joueurs du nouveau joueur
            std::string newPlayer = "JON " + std::to_string(player->getID()) + " " + player->getName() + "\r\n";
            player->getSalon()->CreateMessage(newPlayer, Type::CONNECT, player->getID());

            /* start game */
            if (player->getSalon()->HowManyPlayers() >= player->getSalon()->HowMaxPlayers()) {
                std::cout << "STARTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << std::endl;
                std::string srt_msg = "SRT\r\n";
                if (debugMode)
                    std::cout << "[DEBUG] OK 2 Players online - Send READY to all players" << std::endl;
                player->getSalon()->CreateMessage(srt_msg, Type::START, player->getID());
            } else {
                std::cout << "Not ready because there's only " << player->getSalon()->HowManyPlayers() << " over " << player->getSalon()->HowMaxPlayers() << std::endl;
            }
            if (debugMode)
                std::cout << "OK Player " << id << " changed his salon to " << salonName << std::endl;
        };
};

#endif /* !JOS_COMMANDS_HPP_ */
