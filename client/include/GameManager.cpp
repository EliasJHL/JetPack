/*
** GameManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:46:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:03:07 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "GameManager.hpp"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::init_game(int ac, char **av)
{
    
}

void GameManager::close_connection(void)
{
    
}

void GameManager::run_game(void)
{
    while (mWindow.isOpen()) {
        
    }
}

void GameManager::create_window(void)
{
    sf::VideoMode mode(800, 600, 32);
    mWindow.create(mode, "JetPack Client", sf::Style::Close);
}

void GameManager::handle_events(void)
{
    while (mWindow.pollEvent(mEvent)) {
        if (mEvent.type == sf::Event::Closed)
            mWindow.close();
        //if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
    }
}

void GameManager::handle_server(void)
{
    
}

void GameManager::draw(void)
{
    
}