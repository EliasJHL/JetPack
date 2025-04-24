/*
** main.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/src
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:40:07 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 23:05:02 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include <string>
#include <iostream>
#include <thread>
#include "GameManager.hpp"

int main(int ac, char **av)
{
    GameManager manager;

    try {
        manager.init_game(ac, av);
        std::thread g(&GameManager::run_game, &manager);
        g.join();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
}