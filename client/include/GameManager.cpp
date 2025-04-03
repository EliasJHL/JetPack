/*
** GameManager.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:46:11 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Thu Apr 2 10:17:53 2025 Elias Josué HAJJAR LLAUQUEN
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
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
        std::cout << "serv: " << buffer << std::endl;
        std::string command(buffer);
        std::smatch m;

        std::regex const e{"^PLY (\\d) ([0-9]*\\.[0-9]+) ([0-9]*\\.[0-9]+) (\\d)$"};
        if (std::regex_search(command, m, e)) {
            int id = std::atoi(m[1].str().c_str()); // id
            float x = std::atof(m[2].str().c_str()); // x
            float y = std::atof(m[3].str().c_str()); // y
            int coins = std::atoi(m[4].str().c_str()); // coins

            Player *player = mPlayerManager->getPlayer(id);
            if (player == nullptr || mPlayerID != id) {
                continue;
            }
            player->setPosition({x, y});
            //player->addCoins(coins);
        }
        std::regex const e2{"JON \d [A-Za-z]+$"};
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                pos.second -= 0.05;
                player->setAction(1, 2); // Jump action (second row)
            } else if (pos.second < FLOOR) {
                pos.second += 0.03;
                player->setAction(1, 1); // Default action
            } else {
                pos.second += 0.03;
                player->setAction(0, 0); // Idle action (first row)
            }

            player->setPosition({pos.first, pos.second});
            player->updateAnimation(); // Ensure this is called
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