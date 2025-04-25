/*
** PlayersManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 11:06:30 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:10:19 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef PLAYERSMANAGER_HPP_
# define PLAYERSMANAGER_HPP_
#include <unordered_map>
#include <vector>
#include <mutex>
#include "Player.hpp"

class PlayersManager {
    public:
        static PlayersManager *getInstance(void);
        int createPlayer(const std::string &name, int socket);
        void removePlayer(int id);
        Player *getPlayer(int id);
        std::vector<Player *> getAllPlayers(void);
        std::vector<Player *> getAllPlayers(void) const;
        std::vector<Player *> getReadyPlayer(void);
        std::vector<Player *> getReadyPlayer(void) const;
    private:
        PlayersManager() {mNextPlayerID = 1;};
        ~PlayersManager() = default;

        PlayersManager(PlayersManager &other) = delete;
        void operator=(const PlayersManager &) = delete;
        static std::mutex mMutex;
        std::unordered_map<unsigned int, std::unique_ptr<Player>> mPlayers;
        int mNextPlayerID;
        static PlayersManager *mPlayersManager;
};

#endif /* !PLAYERSMANAGER_HPP_ */
