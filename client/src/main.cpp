/*
** main.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/src
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:40:07 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:39:58 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include <string>
#include <iostream>
#include "GameManager.hpp"

int main(int ac, char **av)
{
    GameManager manager;

    try {
        manager.init_game(ac, av);
        manager.run_game();
        manager.close_connection();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }
}