/*
** PlayersManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:30:56 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:15:14 2025 Elias Josué HAJJAR LLAUQUEN
*/

// basé sur le sigleton

#ifndef PLAYERSMANAGER_HPP_
# define PLAYERSMANAGER_HPP_

#include <unordered_map>
#include <vector>
#include "Player.hpp"
#include <memory>

class PlayersManager {
    public:
        PlayersManager();
        ~PlayersManager();
        static PlayersManager *getInstance(void);
        void createPlayer(const std::string &name, int id);
        void removePlayer(int id);
        Player *getPlayer(int id);
        std::vector<Player *> getAllPlayers(void);
    private:
        PlayersManager() {};
        ~PlayersManager() = default;

        PlayersManager(PlayersManager &other) = delete;
        void operator=(const PlayersManager &) = delete;
        std::unordered_map<unsigned int, std::unique_ptr<Player>> mPlayers;
        static PlayersManager *mPlayersManager;
};

#endif /* !PLAYERSMANAGER_HPP_ */
