/*
** PlayersManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:30:56 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:02:12 2025 Elias Josué HAJJAR LLAUQUEN
*/

// basé sur le sigleton

#ifndef PLAYERSMANAGER_HPP_
# define PLAYERSMANAGER_HPP_

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "Player.hpp"

class PlayersManager {
    public:
        static PlayersManager *getInstance(void);
        void createPlayer(const std::string &name, int id, sf::Vector2f viewPos);
        void removePlayer(int id);
        Player *getPlayer(int id);
        std::vector<Player *> getAllPlayers(void);
    private:
        PlayersManager() {};
        ~PlayersManager() = default;

        PlayersManager(PlayersManager &other) = delete;
        void operator=(const PlayersManager &) = delete;
        std::unordered_map<int, std::unique_ptr<Player>> mPlayers;
        static PlayersManager *mPlayersManager;
        static std::mutex mMutex;
};

#endif /* !PLAYERSMANAGER_HPP_ */
