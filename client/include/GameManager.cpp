/*
** GameManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:46:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 23:38:52 2025 Elias Josué HAJJAR LLAUQUEN
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
    mHasUsername = false;
    mFont.loadFromFile("./client/ressources/font/font.ttf");
    mPlayerInputDisplay.setFont(mFont);
    mPlayerInputDisplay.setCharacterSize(15);
    mPlayerInputDisplay.setString("");
}

void GameManager::close_connection(void)
{
}

void GameManager::run_game(void)
{
    create_window();
    while (mWindow.isOpen()) {
        handle_events();
        if (mHasUsername) {
            std::pair<float, float> pos = mPlayerManager->getPlayer(mPlayerID)->getPosition();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                pos.second -= 0.05;
            else
                pos.second += 0.03;
            mPlayerManager->getPlayer(mPlayerID)->setPosition({pos.first, pos.second});
        }
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
        if (mEvent.type == sf::Event::TextEntered && !mHasUsername) {
            if (mEvent.text.unicode == '\b' && !mInput.isEmpty()) {
                mInput.erase(mInput.getSize() - 1, 1);
            } else if (mEvent.text.unicode != '\b' && mEvent.text.unicode != '\n' && mEvent.text.unicode != '\r' && mInput.getSize() < 20) {
                mInput += mEvent.text.unicode;
            }
            mPlayerInputDisplay.setPosition((mWindow.getSize().x - mPlayerInputDisplay.getGlobalBounds().width) / 2, 300);
            mPlayerInputDisplay.setString(mInput);
        }
        if (mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::Return && !mHasUsername) {
            if (mInput.isEmpty()) {
                std::cout << "Username required" << std::endl;
                continue;
            }
            if (mInput.getSize() < 5) {
                std::cout << "Username too short" << std::endl;
                continue;
            }
            mPlayerUsername = std::string(mInput);
            std::cout << "OK : " << mPlayerUsername << std::endl;
            mPlayerManager->createPlayer(mPlayerUsername, mPlayerID);
            mHasUsername = true;
        }
    }
}

void GameManager::handle_server(void)
{
    
}

void GameManager::draw(void)
{
    mWindow.clear(sf::Color::Black);
    if (!mHasUsername) {
        mWindow.draw(mPlayerInputDisplay);
    } else {
        Player* player = mPlayerManager->getPlayer(mPlayerID);
        if (player == nullptr)
            std::cout << "NULL" << std::endl;
        mWindow.draw(player->getPlayerSprite());
    }
    mWindow.display();
}