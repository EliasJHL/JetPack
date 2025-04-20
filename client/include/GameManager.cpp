/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** GameManager
*/

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
    mPosThread = false;
    mSoundManager.loadSound("coin", "./client/ressources/sounds/coin_pickup_1.wav");
    mSoundManager.loadSound("barrier", "./client/ressources/sounds/dud_zapper_pop.wav");
    mSoundManager.loadSound("jump", "./client/ressources/sounds/jetpack_start.wav");
    mSoundManager.loadSound("fly", "./client/ressources/sounds/jetpack.ogg");
    mSoundManager.loadSound("stopfly", "./client/ressources/sounds/jetpack_stop.wav");
    mSoundManager.loadSound("touchfloor", "./client/ressources/sounds/jetpack_lp.wav");
    mSoundManager.loadMusic("./client/ressources/sounds/theme.ogg");
    mBackground.loadFromFile("./client/ressources/sprites/background.png");
    mSoundManager.setVolume(50, 10);
    mSoundManager.playMusic();
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
        if (mDebugMode) {
            std::cout << "[DEBUG] Sent: " << message << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void GameManager::commandsHandler(void)
{
    char buffer[2048];
    std::string messageBuffer;

    while (mRunning) {
        int bytes = recv(mPlayerSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes == -1) {
            std::cerr << "Error: Failed to receive data from server." << std::endl;
            break;
        } else if (bytes == 0) {
            std::cout << "Server closed the connection." << std::endl;
            exit(0);
        }
        buffer[bytes] = '\0';
        
        messageBuffer += std::string(buffer);

        if (mDebugMode)
            std::cout << "[DEBUG] Received: " << buffer << std::endl;
        
        int pos;
        while ((pos = messageBuffer.find("\r\n")) != std::string::npos) {
            std::string command = messageBuffer.substr(0, pos);
            messageBuffer.erase(0, pos + 2);

            if (command.empty())
                continue;
                        
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
                    std::string coins = parts[4].c_str();

                    Player *player = mPlayerManager->getPlayer(id);
                    if (player == nullptr && id != mPlayerID) {
                        continue;
                    }
                    if (mPlayerID != id && player) {
                        player->updateOnlinePlayersPosition({x * mScaleFactor, y});
                    }
                    if (player) {
                        player->setScore(coins);
                    }
                }
            }
            else if (command.substr(0, 3) == "HIH") {
                std::istringstream iss(command.substr(4));
                iss >> mMapHeight;
                //std::cout << "Map height received: " << mMapHeight << std::endl;
                mScaleFactor = static_cast<float>(mMode.height) / mMapHeight;
            }
            else if (command.substr(0,3) == "WIH") {
                std::istringstream iss(command.substr(4));
                iss >> mMapWidth;
                std::cout << "Map width received: " << mMapWidth << std::endl;
            }
            else if (command == "SRT") {
                std::cout << "Ready received from the server" << std::endl;
                mGameReady = true;
                if (!mPosThread) {
                    std::thread s(&GameManager::posSender, this);
                    s.detach();
                    mPosThread = true;
                }
            }
            else if (command.substr(0, 3) == "JON" && mHasUsername) {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;

                std::cout << command << std::endl;
                
                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }
                
                if (parts.size() >= 3) {
                    int id = std::atoi(parts[1].c_str());
                    std::string name = parts[2];
                    Player *player = mPlayerManager->getPlayer(id);
                    if (player == nullptr && mPlayerID != id) {
                        std::cout << "New player joined! : " << id << " " << name << std::endl;
                        mPlayerManager->createPlayer(name, id);
                    }
                }
            }
            else if (command.substr(0, 3) == "CON") {
                std::stringstream coinStream(command);
                std::string type;
                float x = 0;
                float y = 0;
                coinStream >> type >> x >> y;

                Coin* coin = new Coin();
                coin->setPosition({x * mScaleFactor, y * mScaleFactor});
                coin->getSprite().setScale(mScaleFactor / 220, mScaleFactor / 220);
                mCoins.push_back(coin);

                std::cout << "Coin position: " << x * mScaleFactor << ", " << y * mScaleFactor << std::endl;
            }
            else if (command.substr(0, 3) == "BAR") {
                std::stringstream barrierStream(command);
                std::string type;
                float x, y;
                barrierStream >> type >> x >> y;

                ElectricBarrier* barrier = new ElectricBarrier();
                barrier->setPosition({x * mScaleFactor, y * mScaleFactor});
                barrier->getSprite().setScale(mScaleFactor / mScaleFactor - 0.5, mScaleFactor / mScaleFactor - 0.5);
                mBarriers.push_back(barrier);

                std::cout << "Barrier position: " << x * mScaleFactor << ", " << y * mScaleFactor << std::endl;
            }
            else if (command.substr(0, 3) == "RST") {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;
            
                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }
            
                if (parts.size() >= 2) {
                    int id = std::atoi(parts[1].c_str());
                    Player *player = mPlayerManager->getPlayer(id);
            
                    if (player) {
                        if (mDebugMode)
                            std::cout << "[DEBUG] Player " << id << " map reset triggered." << std::endl;
                        player->setPosition({0, player->getPosition().second});
                        for (auto &coin : mCoins) {
                            coin->toDisplay(true);
                        }
                    }
                }
            }
            else if (command.substr(0, 3) == "COC") {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;
                
                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }

                if (parts.size() >= 4) {
                    int id = std::atoi(parts[1].c_str());
                    float x = std::atof(parts[2].c_str());
                    float y = std::atof(parts[3].c_str());

                    if (id != mPlayerID)
                        continue;

                    for (const auto &coin : mCoins) {
                        std::pair <float, float> pos = coin->getPosition();
                        if (pos.first == x && pos.second == y) {
                            mSoundManager.playSound("coin");
                            coin->toDisplay(false);
                        }
                    }
                }
            }
            else if (command.substr(0, 3) == "DED") {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;
                
                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }

                if (parts.size() >= 2) {
                    int id = std::atoi(parts[1].c_str());

                    Player *player = mPlayerManager->getPlayer(id);

                    player->setDead();

                    mSoundManager.playSound("barrier");
                }
            }
            else if (command.substr(0, 3) == "WIN") {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;
                
                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }

                if (parts.size() >= 2) {
                    int id = std::atoi(parts[1].c_str());

                    Player *player = mPlayerManager->getPlayer(id);

                    if (id != mPlayerID)
                        continue;

                    player->setWin();
                }
            }
            else if (command.substr(0, 3) == "IDP") {
                mPlayerID = std::atoi(command.substr(4).c_str());
                std::cout << "OK - ID given by server is " << mPlayerID << std::endl;
            }
            else if (command.substr(0, 3) == "DEC") {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;

                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }

                if (parts.size() >= 2) {
                    int id = std::atoi(parts[1].c_str());
                    Player* player = mPlayerManager->getPlayer(id);
                    if (player != nullptr) {
                        std::cout << "Player disconnected: " << id << std::endl;
                        mPlayerManager->removePlayer(id);
                    }
                }
            }
            else {
                continue;
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
            std::cout << "Debug mode enabled." << std::endl;
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
    
    // read(mPlayerSocket, data, sizeof(data));
    // int size = read(mPlayerSocket, data, sizeof(data));
    // data[size] = '\0';

    // if (std::string(data).substr(0, 3) == "IDP") {
        
    // } else {
    //     std::cout << "ERROR NOT VALID ID COMMAND" << std::endl;
    // }
    mPlayerManager = mPlayerManager->getInstance();
    mHasUsername = false;
    mGameReady = false;
    mScaleFactor = 60.0f;
    std::thread t(&GameManager::commandsHandler, this);
    t.detach();
}

void GameManager::close_connection(void)
{
}

void GameManager::move_background(void)
{
    Player* player = mPlayerManager->getPlayer(mPlayerID);
    std::pair<float, float> pos;

    if (player == nullptr)
        return;
    pos = player->getPosition();
    sf::Sprite backgroundSprite(mBackground);

    //float offsetX = static_cast<int>(pos.first) % static_cast<int>(mMapWidth * mScaleFactor);

    if (pos.first > 17160) {
        player->setPosition({0, pos.second});
    }
    backgroundSprite.setScale(2.5f, 2.5f);
    backgroundSprite.setPosition(-4315, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(4315, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(0, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(8630, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(12945, -70);
    mWindow.draw(backgroundSprite);
    backgroundSprite.setPosition(17160, -70);
    mWindow.draw(backgroundSprite);
}

void GameManager::handleAnimations(void)
{
    if (!mHasUsername || !mGameReady)
        return;

    Player* player = mPlayerManager->getPlayer(mPlayerID);
    std::pair<float, float> pos = player->getPosition();
    bool SpaceKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool IsOnGround = (pos.second >= FLOOR);
    bool IsPlayerDead = player->isDead();
    bool IsFlying;

    if (SpaceKeyPressed && !IsPlayerDead) {
        if (IsOnGround) {
            if (!mSoundManager.isSoundPlaying("jump"))
                mSoundManager.playSound("jump");
        } else {
            if (!mSoundManager.isSoundPlaying("fly"))
                mSoundManager.playSound("fly", true);
        }
        pos.second -= 4;
        player->setAction(1, 2);
    } else if (pos.second < FLOOR) {
        pos.second += 5;
        if (!IsPlayerDead)
            player->setAction(1, 1);
        else
            player->setAction(3, 0);
    } else {
        if (IsFlying) {
            mSoundManager.stopSound("fly");
            mSoundManager.playSound("stopfly");
        }
        if (!IsOnGround) {
            pos.second += 5;
            if (!IsPlayerDead)
                player->setAction(1, 1);
            else
                player->setAction(3, 0);
        } else {
            if (!IsPlayerDead)
                player->setAction(0, 0);
            else
                player->setAction(3, 0);
        }
    }

    player->updateScoreText();
    pos.first += 4;
    player->setPosition({pos.first, pos.second});
    player->updateAnimation();
    mView.setCenter(pos.first, mView.getCenter().y);
    mWindow.setView(mView);

    // Update Other players animations
    for (Player *p : mPlayerManager->getAllPlayers()) {
        bool isLocalPlayer = (p->getID() == mPlayerID);
        if (isLocalPlayer)
            continue;
        pos = p->getPosition();
        IsOnGround = (pos.second >= FLOOR);
        IsPlayerDead = p->isDead();

        if (pos.second < FLOOR) {
            if (!IsPlayerDead)
                p->setAction(1, 2);
            else
                p->setAction(3, 0);
        } else {
            if (!IsOnGround) {
                if (!IsPlayerDead)
                    p->setAction(1, 1);
                else
                    p->setAction(3, 0);
            } else {
                if (!IsPlayerDead)
                    p->setAction(0, 0);
                else
                    p->setAction(3, 0);
            }
        }
    }

    // Update Animations
    for (Player *p : mPlayerManager->getAllPlayers()) {
        bool isLocalPlayer = (p->getID() == mPlayerID);
        if (!isLocalPlayer)
            p->updateAnimation();
    }
}

void GameManager::run_game(void) {
    create_window();
    mWindow.setFramerateLimit(60);

    while (mWindow.isOpen()) {
        handle_events();
        handleAnimations();
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
            std::string m = "DEC\r\n";
            send(mPlayerSocket, m.c_str(), m.length(), 0);
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
        move_background();
        for (IEntity* entity : entities) {
            sf::Sprite sprite = entity->getSprite();
            if (Player* player = dynamic_cast<Player*>(entity)) {
                if (player->getID() != mPlayerID) {
                    sf::Color color = sprite.getColor();
                    color.a = 128;
                    sprite.setColor(color);
                    sprite.setPosition(player->getPosition().first / mScaleFactor - sprite.getGlobalBounds().width / 2, player->getPosition().second);
                    mMessageText.setPosition({player->getPosition().first / mScaleFactor - mMessageText.getGlobalBounds().width / 2, player->getPosition().second - mMessageText.getGlobalBounds().height - 10});
                    mMessageText.setCharacterSize(10);
                    mMessageText.setString(player->getName());
                    mWindow.draw(mMessageText);
                } else {
                    mWindow.draw(player->getScoreText());
                }
            }
            mWindow.draw(sprite);
        }
        mMessageText.setCharacterSize(30);
        if (mPlayerManager->getPlayer(mPlayerID)->isDead()) {
            mMessageText.setString("GAME OVER :(");
            mMessageText.setPosition((mPlayerManager->getPlayer(mPlayerID)->getPosition().first - mMessageText.getGlobalBounds().width), 200);
            mWindow.draw(mMessageText);
        }
        if (mPlayerManager->getPlayer(mPlayerID)->isWin()) {
            mMessageText.setString("VICTORY !!!");
            mMessageText.setPosition((mPlayerManager->getPlayer(mPlayerID)->getPosition().first - mMessageText.getGlobalBounds().width), 200);
            mWindow.draw(mMessageText);
        }
    }
    mWindow.display();
}