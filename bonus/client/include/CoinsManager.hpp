/*
** CoinsManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:30:56 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:02:12 2025 Elias Josué HAJJAR LLAUQUEN
*/

// basé sur le sigleton

#ifndef COINSMANAGER_HPP
# define COINSMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "Coin.hpp"

class CoinsManager {
    public:
        static CoinsManager *getInstance(void);
        void createCoin(const std::string &name, int id);
        void removeCoin(int id);
        Coin *getCoin(int id);
        std::vector<Coin *> getAllCoins(void);
    private:
        CoinsManager() {};
        ~CoinsManager() = default;

        CoinsManager(CoinsManager &other) = delete;
        void operator=(const CoinsManager &) = delete;
        std::unordered_map<int, std::unique_ptr<Coin>> mCoins;
        static CoinsManager *mCoinsManager;
        static std::mutex mMutex;
};

#endif /* !COINSMANAGER_HPP */
