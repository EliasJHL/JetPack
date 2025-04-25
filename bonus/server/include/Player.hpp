/*
** Player.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 10:56:51 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 14:43:09 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#include <memory>
#include <string>
#include "NetworkObserver.hpp"
#include <mutex>
#include <vector>
#include <algorithm>

class Player {
    public:
        Player(int id, const std::string &name, int socket);
        ~Player();
        
        int getID(void) const;
        const std::string &getName(void) const;
        bool isConnected(void) const;
        NetworkObserver *getObserver();
        NetworkSalon *getSalon();
        int getPlayerSocket();
        int getCoins();

        const std::pair<float, float> &getPosition(void) const;
        void setPosition(std::pair<float, float> pos);
        void setConnection(bool connexion);
        void setSalon(NetworkSalon &salon, bool debugMode);
        void setPlayerSocket(int fd);
        void setPlayerName(const std::string &name);
        void addCoins(int nb, std::pair<float, float> coin, bool debugMode);
        void clearCollectedCoins();
        
        void playerWin(void);
        void playerDie(void);
        
        bool isPlayerDied(void);
        bool isPlayerWin(void);
        
        bool isToDelete() {
            std::lock_guard<std::mutex> lock(mAccessMutex);
            return mToDelete;
        };
        void ToDelete() {
            std::lock_guard<std::mutex> lock(mAccessMutex);
            mToDelete = true;
        };
        private:
        int mPlayerID;
        int mPlayerCoin;
        int mPlayerSocket;
        bool mWin;
        bool mDied;
        bool mToDelete;
        bool mConnected;
        std::string mPlayerName;
        std::mutex mAccessMutex;
        NetworkObserver *mObserver;
        std::pair<float, float> mCoordinates;
        std::vector<std::pair<float, float>> mCollectedCoins;
};

#endif /* !PLAYER_HPP_ */
