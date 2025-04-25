/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** GameManager
*/

#include "GameManager.hpp"

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
    mSoundManager.setVolume(40, 10);
    mSoundManager.playMusic();

    mRect.setSize({1200, 150});
    mRect.setFillColor({0, 0, 0, 255});

    mAlreadyJoining = false;
    mCreateServerPort = 2;

    mServerNameDisplay.setFont(mFont);

    /* START MENU */
    mStartButtons.emplace_back(std::make_unique<Bouton>(600, 250, 30, "Create Room", true, true, Color(255, 255, 255, 255)));
    mStartButtons.emplace_back(std::make_unique<Bouton>(600, 300, 30, "Join Room", true, true, Color(255, 255, 255, 255)));
    mStartButtons.emplace_back(std::make_unique<Bouton>(600, 350, 30, "Quit", true, true, Color(255, 255, 255, 255)));

    /* CREATE GAME */
    mCreateButtons.emplace_back(std::make_unique<Bouton>(100, 340, 30, "M", false, true, Color(255, 255, 255, 255)));
    mCreateButtons.emplace_back(std::make_unique<Bouton>(180, 340, 30, "P", false, true, Color(255, 255, 255, 255)));
    mCreateButtons.emplace_back(std::make_unique<Bouton>(100, 500, 20, "[Create Room]", false, true, Color(255, 255, 255, 255)));
    
    /* JOIN SERVER */
    mJoinButtons.emplace_back(std::make_unique<Bouton>(900, 50, 20, "Refresh", true, true, Color(255, 255, 255, 255)));
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

    mGameMode = START;
    mLobbyMenu = BASE;

    mPlayerManager = mPlayerManager->getInstance();
    mHasUsername = false;
    mGameReady = false;
    mScaleFactor = 60.0f;
    mViewPos.x = 0;
    mViewPos.y = mView.getCenter().y;
    std::thread t(&GameManager::commandsHandler, this);
    t.detach();
}

void GameManager::create_window(void)
{
    mWindow.create(mMode, "JetPack Client", sf::Style::Close);
}

void GameManager::run_game(void) {
    create_window();
    mWindow.setFramerateLimit(60);

    while (mWindow.isOpen()) {
        if (mGameMode == LOBBY) {
            handle_menu_events();
        } else {
            handle_events();
            handleAnimations();
        }
        draw();

    }
    mRunning = false;
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

        /* USERNAME */
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
            mPlayerManager->createPlayer(mPlayerUsername, mPlayerID, mViewPos);
            send(mPlayerSocket, std::string("SNA " + mPlayerUsername).c_str(), std::string("SNA " + mPlayerUsername).length(), 0);
            mHasUsername = true;
            mGameMode = LOBBY;
        }
    }
}

void GameManager::StartDraw(void)
{
    sf::Text mMessageText;
    mMessageText.setFont(mFont);
    mMessageText.setCharacterSize(30);
    mMessageText.setFillColor(sf::Color::White);

    mMessageText.setString("Enter your username:");
    mMessageText.setPosition((mWindow.getSize().x - mMessageText.getGlobalBounds().width) / 2, 200);
    mWindow.draw(mMessageText);
    mWindow.draw(mPlayerInputDisplay);
}

void GameManager::GameDraw(std::vector<IEntity*> entities)
{
    sf::Text mMessageText;
    mMessageText.setFont(mFont);
    mMessageText.setCharacterSize(30);
    mMessageText.setFillColor(sf::Color::White);

    if (!mGameReady) {
        mMessageText.setString("Waiting a second player...");
        mMessageText.setPosition((mWindow.getSize().x - mMessageText.getGlobalBounds().width) / 2, 200);
        mWindow.draw(mMessageText);
    } else {
        move_background();
        for (IEntity* entity : entities) {
            sf::Sprite sprite = entity->getSprite();
            if (Player* player = dynamic_cast<Player*>(entity)) {
                if (player == nullptr)
                    continue;
                if (player->getID() != mPlayerID) {
                    sf::Color color = sprite.getColor();
                    color.a = 128;
                    sprite.setColor(color);
                    sprite.setPosition(player->getPosition().first / mScaleFactor, player->getPosition().second);
                    mMessageText.setPosition({player->getPosition().first / mScaleFactor + 55, player->getPosition().second - mMessageText.getGlobalBounds().height - 10});
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
        Player* localPlayer = mPlayerManager->getPlayer(mPlayerID);
        if (localPlayer != nullptr) {
            if (localPlayer->isDead()) {
                mMessageText.setColor({255, 100, 100, 255});
                mMessageText.setString("GAME OVER");
            }
            if (localPlayer->isWin()) {
                mMessageText.setColor({100, 255, 100, 255});
                mMessageText.setString("VICTORY");
            }
            mWindow.draw(mMessageText);
            mMessageText.setPosition((mViewPos.x - (mMessageText.getGlobalBounds().width / 2)), 200);
            if (localPlayer->isDead() || localPlayer->isWin()) {
                mRect.setPosition({mViewPos.x - 600, 140});
                mWindow.draw(mRect);
            }
            mWindow.draw(mMessageText);
            mMessageText.setColor({255, 255, 255, 255});
        }
    }
}

void GameManager::draw(void)
{
    std::vector<IEntity*> entities;

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
    if (mGameMode == START) {
        StartDraw();
    } else if (mGameMode == LOBBY) {
        MenuDraw();
    } else {
        GameDraw(entities);
    }
    mWindow.display();
}