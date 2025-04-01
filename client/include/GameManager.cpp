/*
** GameManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:46:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:59:40 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "GameManager.hpp"

void GameManager::init_game(int ac, char **av)
{
    // gérer le -h -p et -d
    char data[2048];
    mPlayerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    mAddressControl.sin_family = AF_INET;
    mAddressControl.sin_port = htons(std::atoi(av[2]));
    mAddressControl.sin_addr.s_addr = inet_addr(av[1]);

    if (connect(mPlayerSocket, (struct sockaddr *)&mAddressControl, sizeof(mAddressControl)) == -1)
        throw std::runtime_error("Impossible to connect");
    
    int size = read(mPlayerSocket, data, sizeof(data));
    data[size] = '\0';

    mPlayerID = std::atoi(std::string(data).substr(3).c_str());
    mPlayerManager = mPlayerManager->getInstance();
}

void GameManager::close_connection(void)
{
}

void GameManager::run_game(void)
{
    create_window();
    while (mWindow.isOpen()) {
        handle_events();
        draw();
    }
}

void GameManager::create_window(void)
{
    sf::VideoMode mode(1200, 600, 32);
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
    mWindow.clear(sf::Color::Black);
    mWindow.display();
}