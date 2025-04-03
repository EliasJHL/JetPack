/*
** GameManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:46:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 13:38:07 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "GameManager.hpp"

void GameManager::test_send(void)
{
    while (true) {
        float x, y;
        x = mPlayerManager->getPlayer(mPlayerID)->getPosition().first;
        y = mPlayerManager->getPlayer(mPlayerID)->getPosition().second;
        std::string position_message = "POS " + std::to_string(x) + " " + std::to_string(y);
        write(mPlayerSocket, position_message.c_str(), position_message.size());
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void GameManager::test_server(void)
{
    char buffer[2048];

    while (true) {
        int bytes = recv(mPlayerSocket, buffer, sizeof(buffer), 0);
        if (bytes == -1) {
            std::cerr << "Error: Failed to receive data from server." << std::endl;
            break;
        } else if (bytes == 0) {
            std::cout << "Server closed the connection." << std::endl;
            break;
        }
        buffer[bytes] = '\0';
        std::string command(buffer);
        std::smatch m;
        std::regex const e{"PLY\\s+(\\d+)\\s+([\\d.-]+)\\s+([\\d.-]+)\\s+(\\d+)"};
        if (std::regex_search(command, m, e)) {
            int id = std::atoi(m[1].str().c_str());
            float x = std::atof(m[2].str().c_str());
            float y = std::atof(m[3].str().c_str());
            int coins = std::atoi(m[4].str().c_str());

            Player *player = mPlayerManager->getPlayer(id);
            if (player == nullptr || mPlayerID == id) {
                continue;
            }
            player->setPosition({x, y});
            //player->addCoins(coins);
        }
        std::regex const e2{"^JON\\s+(\\d+)\\s+([A-Za-z0-9_-]+)\r\n$"};
        if (std::regex_search(command, m, e2)) {
            int id = std::atoi(m[1].str().c_str());
            std::string name = m[2].str();
            Player *player = mPlayerManager->getPlayer(id);
            if (player == nullptr) {
                mPlayerManager->createPlayer(name, id);
            } else {
                player->setName(name);
            }
        }
    }
}

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
    std::thread t(&GameManager::test_server, this);
    t.detach();
}

void GameManager::close_connection(void)
{
}

void GameManager::run_game(void) {
    create_window();
    while (mWindow.isOpen()) {
        handle_events();
        if (mHasUsername) {
            Player* player = mPlayerManager->getPlayer(mPlayerID);
            std::pair<float, float> pos = player->getPosition();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mWindow.hasFocus()) {
                pos.second -= 0.05;
                player->setAction(1, 2);
            }
            player->setPosition({pos.first, pos.second});
        }
        std::vector<Player*> players = mPlayerManager->getAllPlayers();
        for (int i = 0; i < players.size(); i++) {
            std::pair<float, float> pos = players[i]->getPosition();
            if (pos.second < FLOOR) {
                pos.second += 0.03;
                players[i]->setAction(1, 1);
            } else {
                pos.second += 0.03;
                players[i]->setAction(0, 0);
            }
            players[i]->updateAnimation();
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
            if (mInput.getSize() < 2) {
                std::cout << "Username too short" << std::endl;
                continue;
            }
            mPlayerUsername = std::string(mInput);
            std::cout << "OK : " << mPlayerUsername << std::endl;
            mPlayerManager->createPlayer(mPlayerUsername, mPlayerID);
            send(mPlayerSocket, std::string("SNA " + mPlayerUsername).c_str(), std::string("SNA " + mPlayerUsername).length(), 0);
            mHasUsername = true;
            std::thread s(&GameManager::test_send, this);
            s.detach();
        }
    }
}

void GameManager::handle_server(void)
{
}

void GameManager::draw(void)
{
    std::vector<Player*> players = mPlayerManager->getAllPlayers();
    
    mWindow.clear(sf::Color::Black);
    if (!mHasUsername) {
        mWindow.draw(mPlayerInputDisplay);
    } else {
        for (int i = 0; i < players.size(); i++) {
            //std::cout << players[i]->getPosition().first << " " << players[i]->getPosition().second << std::endl;
            mWindow.draw(players[i]->getPlayerSprite());
            //std::cout << "Dessin du joueur " << players[i]->getID() << std::endl;
        }
    }
    mWindow.display();
}