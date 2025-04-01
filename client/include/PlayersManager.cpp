/*
** PlayersManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 21:05:44 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:15:19 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "PlayersManager.hpp"

PlayersManager* PlayersManager::mPlayersManager = nullptr;

PlayersManager *PlayersManager::getInstance()
{
    if (mPlayersManager == nullptr)
        mPlayersManager = new PlayersManager();
    return mPlayersManager;
}

void PlayersManager::createPlayer(const std::string &name, int id)
{
    mPlayers[id] = std::make_unique<Player>(id, name);
}

void PlayersManager::removePlayer(int id)
{
    mPlayers.erase(id);
}

Player *PlayersManager::getPlayer(int id)
{
    auto it = mPlayers.find(id);
    if (it != mPlayers.end())
        return it->second.get();
    return nullptr;
}

std::vector<Player *> PlayersManager::getAllPlayers(void)
{
    std::vector<Player *> players;
    for (const auto&[key, player] : mPlayers) {
        players.push_back(player.get());
    }
    return players;
}