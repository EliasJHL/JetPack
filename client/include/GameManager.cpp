// HEADER
#include "GameManager.hpp"
#include <sstream>
#include <iomanip>

GameManager::GameManager()
{
    mView.setCenter(0, FLOOR - 150);
    mView.setSize(1200, 600);
    mMode.width = 1200;
    mMode.height = 600;
    mMode.bitsPerPixel = 32;
    mFont.loadFromFile("./client/ressources/font/jetpack_font.ttf");
    mPlayerInputDisplay.setFont(mFont);
    mPlayerInputDisplay.setCharacterSize(15);
    mPlayerInputDisplay.setString("");
    mGameReady = false;
}

void GameManager::posSender(void)
{
    Player *p = mPlayerManager->getPlayer(mPlayerID);
    std::string message;
    float x, y;

    while (mRunning) {
        x = p->getPosition().first;
        y = p->getPosition().second;
        std::ostringstream oss;
        oss << "POS " << std::fixed << std::setprecision(2) << x << " " << y;
        message = oss.str();
        send(mPlayerSocket, message.c_str(), message.size(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

void GameManager::commandsHandler(void)
{
    char buffer[2048];

    while (mRunning) {
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
        if (command.substr(0, 3) == "PLY" && mHasUsername && mGameReady) {
            std::stringstream messageStream(command);
            std::vector<std::string> parts;
            std::string m;
            
            while (std::getline(messageStream, m, ' ')) {
                parts.push_back(m);
            }

            if (parts.size() >= 5) {
                int id = std::atoi(parts[1].c_str());
                float x = std::atof(parts[2].c_str());
                float y = std::atof(parts[3].c_str());
                int coins = std::atoi(parts[4].c_str());

                Player *player = mPlayerManager->getPlayer(id);
                if (player == nullptr) {
                    std::cout << "Unknow player" << std::endl;
                    mPlayerManager->createPlayer("Dummy", id);
                    player = mPlayerManager->getPlayer(id);
                }
                if (mPlayerID != id && player) {
                    std::cout << "Update pos for " << id << " " << x << " " << y << std::endl;
                    player->updateOnlinePlayersPosition({x * mScaleFactor, y});
                    //player->getSprite().setScale(mScaleFactor / 70, mScaleFactor / 70);
                    // set coins
                }
            }
        }
        if (command.substr(0, 3) == "HIH") {
            mMapHeight = std::atoi(command.c_str() + 4);
            std::cout << "Map height received: " << mMapHeight << std::endl;
            mScaleFactor = static_cast<float>(mMode.height) / mMapHeight;
        }
        if (command.substr(0, 3) == "SRT") {
            std::cout << "OKOKOKOKOKOKOKOK" << std::endl;
            std::thread s(&GameManager::posSender, this);
            s.detach();
            mGameReady = true;
        }
        if (command.substr(0, 3) == "JON" && mHasUsername) {
            std::stringstream messageStream(command);
            std::vector<std::string> parts;
            std::string m;

            std::cout << command << std::endl;
            
            while (std::getline(messageStream, m, ' ')) {
                parts.push_back(m);
            }
            int id = std::atoi(parts[0].c_str());
            std::string name = parts[1];
            Player *player = mPlayerManager->getPlayer(id);
            if (player == nullptr && mPlayerID != id) {
                mPlayerManager->createPlayer(name, id);
            }
        }
        std::stringstream messageStream(command);
        std::string line;
        while (std::getline(messageStream, line)) {
            if (line.substr(0, 3) == "CON") {
                std::stringstream coinStream(line);
                std::string type;
                float x, y;
                coinStream >> type >> x >> y;

                Coin* coin = new Coin();
                coin->setPosition({x * mScaleFactor, y * mScaleFactor});
                coin->getSprite().setScale(mScaleFactor / 220, mScaleFactor / 220);
                mCoins.push_back(coin);

                std::cout << "Coin position: " << x * mScaleFactor << ", " << y * mScaleFactor << std::endl;
                // std::cout << "Coin scale: " << coin->getSprite().getScale().x << ", " << coin->getSprite().getScale().y << std::endl;
            }
            if (line.substr(0, 3) == "BAR") {
                std::stringstream barrierStream(line);
                std::string type;
                float x, y;
                barrierStream >> type >> x >> y;

                ElectricBarrier* barrier = new ElectricBarrier();
                barrier->setPosition({x * mScaleFactor, y * mScaleFactor});
                barrier->getSprite().setScale(mScaleFactor / mScaleFactor - 0.5, mScaleFactor / mScaleFactor - 0.5);
                mBarriers.push_back(barrier);

                std::cout << "Barrier position: " << x * mScaleFactor << ", " << y * mScaleFactor << std::endl;
                // std::cout << "Barrier scale: " << barrier->getSprite().getScale().x << ", " << barrier->getSprite().getScale().y << std::endl;
            }
        }
    }
}

void GameManager::init_game(int ac, char **av)
{
    char data[2048];
    if (ac < 5 || ac > 6)
        throw std::runtime_error("Usage : ./client -h <ip> -p <port> [-d]");
    std::string ip;
    std::string port;
    for (int i = 1; i < ac; i++) {
        if (std::string(av[i]) == "-h" && i + 1 < ac) {
            ip = av[++i];
            if (ip.empty() || !std::regex_match(ip, std::regex("^(\\d{1,3}\\.){3}\\d{1,3}$")))
                throw std::runtime_error("Invalid IP address: " + ip);
        } else if (std::string(av[i]) == "-p" && i + 1 < ac) {
            port = av[++i];
            if (port.empty() || !std::all_of(port.begin(), port.end(), ::isdigit))
                throw std::runtime_error("Invalid port number: " + port);
        } else if (std::string(av[i]) == "-d") {
            this->mDebugMode = true;
        } else {
            throw std::runtime_error("Invalid arguments. Usage : ./client -h <ip> -p <port> [-d]");
        }
    }
    if (ip.empty())
        throw std::runtime_error("Missing -h <ip> argument.");
    if (port.empty())
        throw std::runtime_error("Missing -p <port> argument.");
    mIp = ip;
    mPort = std::atoi(port.c_str());
    if (mPort <= 0 || mPort > 65535)
        throw std::runtime_error("Invalid port number: " + port);
    mPlayerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    mAddressControl.sin_family = AF_INET;
    mAddressControl.sin_port = htons(std::atoi(port.c_str()));
    mAddressControl.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(mPlayerSocket, (struct sockaddr *)&mAddressControl, sizeof(mAddressControl)) == -1)
        throw std::runtime_error("Impossible to connect");
    
    read(mPlayerSocket, data, sizeof(data));
    int size = read(mPlayerSocket, data, sizeof(data));
    data[size] = '\0';

    mPlayerID = std::atoi(std::string(data).substr(4).c_str());
    mPlayerManager = mPlayerManager->getInstance();
    mHasUsername = false;
    mGameReady = false;
    std::thread t(&GameManager::commandsHandler, this);
    t.detach();
}

void GameManager::close_connection(void)
{
}

void GameManager::run_game(void) {
    create_window();
    mWindow.setFramerateLimit(60);
    while (mWindow.isOpen()) {
        handle_events();
        if (mHasUsername && mGameReady) {
            Player* player = mPlayerManager->getPlayer(mPlayerID);
            std::pair<float, float> pos = player->getPosition();
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mWindow.hasFocus()) {
                pos.second -= 2;
                player->setAction(1, 2);
                player->setScore(std::to_string(std::stoi(player->getScore()) + 1));
            } else if (pos.second < FLOOR) {
                pos.second += 1.5;
                player->setAction(1, 1);
            } else {
                pos.second += 1.5;
                player->setAction(0, 0);
            }
            player->updateScoreText();
            pos.first += 4;
            player->setPosition({pos.first, pos.second});
            player->updateAnimation();
            mView.setCenter(pos.first, mView.getCenter().y);
            mWindow.setView(mView);
        }
        std::vector<Player*> players = mPlayerManager->getAllPlayers();
        for (int i = 0; i < players.size(); i++) {
            if (players[i]->getID() != mPlayerID) {
                players[i]->updateAnimation();
            }
        }
        draw();
    }
    mRunning = false;
}

void GameManager::create_window(void)
{
    mWindow.create(mMode, "JetPack Client", sf::Style::Close);
}

void GameManager::handle_events(void)
{
    while (mWindow.pollEvent(mEvent)) {
        if (mEvent.type == sf::Event::Closed) {
            mRunning = false;
            mWindow.close();
        }
        if (mEvent.type == sf::Event::TextEntered && !mHasUsername && !mGameReady) {
            if (mEvent.text.unicode == '\b' && !mInput.isEmpty()) {
                mInput.erase(mInput.getSize() - 1, 1);
            } else if (mEvent.text.unicode != '\b' && mEvent.text.unicode != '\n' && mEvent.text.unicode != '\r' && mInput.getSize() < 20) {
                mInput += mEvent.text.unicode;
            }
            mPlayerInputDisplay.setPosition((mWindow.getSize().x - mPlayerInputDisplay.getGlobalBounds().width) / 2, 300);
            mPlayerInputDisplay.setString(mInput);
        }
        if (mEvent.type == sf::Event::KeyPressed && mEvent.key.code == sf::Keyboard::Return && !mHasUsername && !mGameReady) {
            if (mInput.isEmpty()) {
                std::cout << "Username required" << std::endl;
                continue;
            }
            if (mInput.getSize() < 2) {
                std::cout << "Username too short" << std::endl;
                continue;
            }
            mPlayerUsername = std::string(mInput);
            mPlayerManager->createPlayer(mPlayerUsername, mPlayerID);
            send(mPlayerSocket, std::string("SNA " + mPlayerUsername).c_str(), std::string("SNA " + mPlayerUsername).length(), 0);
            mHasUsername = true;
        }
    }
}

void GameManager::handle_server(void)
{
}

void GameManager::draw(void)
{
    std::vector<IEntity*> entities;
    sf::Text mMessageText;
    mMessageText.setFont(mFont);
    mMessageText.setCharacterSize(30);
    mMessageText.setFillColor(sf::Color::White);

    for (Coin* coin : mCoins) {
        coin->updateAnimation();
        entities.push_back(coin);
    }
    for (ElectricBarrier* barrier : mBarriers) {
        barrier->updateAnimation();
        entities.push_back(barrier);
    }

    std::vector<Player*> players = mPlayerManager->getAllPlayers();
    entities.insert(entities.end(), players.begin(), players.end());

    mWindow.clear(sf::Color::Black);
    if (!mHasUsername) {
        mMessageText.setString("Enter your username:");
        mMessageText.setPosition((mWindow.getSize().x - mMessageText.getGlobalBounds().width) / 2, 200);
        mWindow.draw(mMessageText);
        mWindow.draw(mPlayerInputDisplay);
    } else if (!mGameReady) {
        mMessageText.setString("Waiting a second player...");
        mMessageText.setPosition((mWindow.getSize().x - mMessageText.getGlobalBounds().width) / 2, 200);
        mWindow.draw(mMessageText);
    } else {
        for (IEntity* entity : entities) {
            sf::Sprite sprite = entity->getSprite();
            if (Player* player = dynamic_cast<Player*>(entity)) {
                if (player->getID() != mPlayerID) {
                    sf::Color color = sprite.getColor();
                    color.a = 128;
                    sprite.setColor(color);
                    sprite.setPosition(player->getPosition().first / mScaleFactor - sprite.getGlobalBounds().width / 2, player->getPosition().second);
                } else {
                    mWindow.draw(player->getScoreText());
                }
            }
            mWindow.draw(sprite);
        }
    }
    mWindow.display();
}