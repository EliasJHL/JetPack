/*
** PlayersManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 11:21:17 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:10:17 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "PlayersManager.hpp"
#include "PlayersManager.hpp"

std::mutex PlayersManager::mMutex;
PlayersManager* PlayersManager::mPlayersManager = nullptr;

PlayersManager *PlayersManager::getInstance() {
    std::lock_guard<std::mutex> guard(mMutex);
    if (mPlayersManager == nullptr)
        mPlayersManager = new PlayersManager();
    return mPlayersManager;
}

int PlayersManager::createPlayer(const std::string &name, int socket) {
    std::lock_guard<std::mutex> guard(mMutex);
    mPlayers[mNextPlayerID] = std::make_unique<Player>(mNextPlayerID, name, socket);
    return mNextPlayerID++;
}

void PlayersManager::removePlayer(int id) {
    std::lock_guard<std::mutex> guard(mMutex);
    auto it = mPlayers.find(id);
    if (it != mPlayers.end()) {
        close(it->second.get()->getPlayerSocket());
    }
    mPlayers.erase(id);
}

Player *PlayersManager::getPlayer(int id) {
    std::lock_guard<std::mutex> guard(mMutex);
    auto it = mPlayers.find(id);
    if (it != mPlayers.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Player *> PlayersManager::getAllPlayers(void) {
    std::lock_guard<std::mutex> guard(mMutex);
    std::vector<Player *> ret;
    for (const auto& [key, player] : mPlayers) {
        ret.push_back(player.get());
    }
    return ret;
}

std::vector<Player *> PlayersManager::getAllPlayers(void) const {
    std::lock_guard<std::mutex> guard(mMutex);
    std::vector<Player *> ret;
    for (const auto& [key, player] : mPlayers) {
        ret.push_back(player.get());
    }
    return ret;
}

std::vector<Player *> PlayersManager::getReadyPlayer(void)
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::vector<Player *> ret;
    for (const auto& [key, player] : mPlayers) {
        if (player.get()->getName() != "Dummy" && !player->isToDelete() && player != nullptr) {
            ret.push_back(player.get());
        }
    }
    return ret;
};

std::vector<Player *> PlayersManager::getReadyPlayer(void) const
{
    std::lock_guard<std::mutex> guard(mMutex);
    std::vector<Player *> ret;
    for (const auto& [key, player] : mPlayers) {
        if (player.get()->getName() != "Dummy") {
            ret.push_back(player.get());
        }
    }
    return ret;
};