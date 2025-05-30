/*
** PlayersManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 21:05:44 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 14:55:12 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "PlayersManager.hpp"
#include <iostream>

std::mutex PlayersManager::mMutex;
PlayersManager* PlayersManager::mPlayersManager = nullptr;

PlayersManager *PlayersManager::getInstance()
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (mPlayersManager == nullptr)
        mPlayersManager = new PlayersManager();
    return mPlayersManager;
}

void PlayersManager::createPlayer(const std::string &name, int id, sf::Vector2f viewPos)
{
    std::lock_guard<std::mutex> lock(mMutex);
    std::cout << id << std::endl;
    auto it = mPlayers.find(id);
    if (it != mPlayers.end()) {
        std::cout << "Attention: Un joueur avec l'ID " << id << " existe déjà!" << std::endl;
        return;
    }
    mPlayers[id] = std::make_unique<Player>(id, name, viewPos);
}

void PlayersManager::removePlayer(int id)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mPlayers.erase(id);
}

Player *PlayersManager::getPlayer(int id)
{
    std::lock_guard<std::mutex> lock(mMutex);
    auto it = mPlayers.find(id);
    if (it != mPlayers.end())
        return it->second.get();
    return nullptr;
}

std::vector<Player *> PlayersManager::getAllPlayers(void)
{
    std::lock_guard<std::mutex> lock(mMutex);
    std::vector<Player *> players;
    for (const auto&[key, player] : mPlayers) {
        players.push_back(player.get());
    }
    return players;
}