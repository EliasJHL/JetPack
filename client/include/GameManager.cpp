// HEADER
#include "GameManager.hpp"
#include <sstream>
#include <iomanip>

void GameManager::test_send(void)
{
    Player *p = mPlayerManager->getPlayer(mPlayerID);
    std::string message;
    float x, y;

    while (true) {
        x = p->getPosition().first;
        y = p->getPosition().second;
        std::ostringstream oss;
        oss << "POS " << std::fixed << std::setprecision(2) << x << " " << y;
        message = oss.str();
        //std::cout << message << std::endl;
        send(mPlayerSocket, message.c_str(), message.size(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

// Suppression des regex qui est trop coûteux
void GameManager::test_server(void)
{
    char buffer[2048];
    // Lire la première ligne pour obtenir la hauteur de la carte
    int bytes = recv(mPlayerSocket, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        std::cerr << "Error: Failed to receive data from server." << std::endl;
        return;
    }
    buffer[bytes] = '\0';
    std::string command(buffer);
    std::stringstream messageStream(command);
    std::string line;
    if (std::getline(messageStream, line) && line.substr(0, 6) == "HEIGHT") {
        std::stringstream heightStream(line);
        std::string type;
        int height;
        heightStream >> type >> height;
        mMapHeight = height; // Stocker la hauteur de la carte
        std::cout << "Map height received: " << mMapHeight << std::endl;
    } else {
        std::cerr << "Error: Expected HEIGHT message but received something else." << std::endl;
        return;
    }
    // Calculer le facteur d'échelle après avoir reçu la hauteur
    float scaleFactor = static_cast<float>(mMode.height) / mMapHeight;
    std::cout << "Window height: " << mWindow.getSize().y << std::endl;
    std::cout << "Scale factor: " << scaleFactor << std::endl;

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
        if (command.substr(0, 3) == "PLY" && mHasUsername) {
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
                    mPlayerManager->createPlayer("Dummy", id);
                    player = mPlayerManager->getPlayer(id);
                }
                if (mPlayerID != id && player) {
                    player->setPosition({x, y});
                    // set coins
                }
            }
        }
        std::stringstream messageStream(command);
        std::string line;
        while (std::getline(messageStream, line)) {
            if (line.substr(0, 4) == "COIN") {
                std::stringstream coinStream(line);
                std::string type;
                float x, y;
                coinStream >> type >> x >> y;

                Coin* coin = new Coin();
                coin->setPosition({x * scaleFactor, y * scaleFactor});
                coin->getSprite().setScale(scaleFactor, scaleFactor);
                mCoins.push_back(coin);

                std::cout << "Coin position: " << x * scaleFactor << ", " << y * scaleFactor << std::endl;
            } else if (line.substr(0, 7) == "BARRIER") {
                std::stringstream barrierStream(line);
                std::string type;
                float x, y;
                barrierStream >> type >> x >> y;

                ElectricBarrier* barrier = new ElectricBarrier();
                barrier->setPosition({x * scaleFactor, y * scaleFactor});
                barrier->getSprite().setScale(scaleFactor, scaleFactor);
                mBarriers.push_back(barrier);

                std::cout << "Barrier position: " << x * scaleFactor << ", " << y * scaleFactor << std::endl;
            }
        }
        if (command.substr(0, 3) == "JON" && mHasUsername) {
            std::stringstream messageStream(command);
            std::vector<std::string> parts;
            std::string m;
            
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
    
    read(mPlayerSocket, data, sizeof(data));
    int size = read(mPlayerSocket, data, sizeof(data));
    data[size] = '\0';

    mPlayerID = std::atoi(std::string(data).substr(4).c_str());
    mPlayerManager = mPlayerManager->getInstance();
    mHasUsername = false;
    mView.setCenter(0, FLOOR - 150);
    mView.setSize(1200, 600);
    mMode.width = 1200;
    mMode.height = 600;
    mMode.bitsPerPixel = 32;
    mFont.loadFromFile("./client/ressources/font/jetpack_font.ttf");
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
                pos.second -= 2;
                player->setAction(1, 2);
            } else if (pos.second < FLOOR) {
                pos.second += 1.5;
                player->setAction(1, 1);
            } else {
                pos.second += 1.5;
                player->setAction(0, 0);
            }
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
        mWindow.setFramerateLimit(60);
        draw();
    }
}

void GameManager::create_window(void)
{
    mWindow.create(mMode, "JetPack Client", sf::Style::Close);
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
    std::vector<IEntity*> entities;

    for (Coin* coin : mCoins) {
        coin->updateAnimation(); // Met à jour l'animation des pièces
        entities.push_back(coin);
    }
    for (ElectricBarrier* barrier : mBarriers) {
        barrier->updateAnimation(); // Met à jour l'animation des barrières
        entities.push_back(barrier);
    }

    std::vector<Player*> players = mPlayerManager->getAllPlayers();
    entities.insert(entities.end(), players.begin(), players.end());

    mWindow.clear(sf::Color::Black);
    if (!mHasUsername) {
        mWindow.draw(mPlayerInputDisplay);
    } else {
        for (IEntity* entity : entities) {
            sf::Sprite sprite = entity->getSprite();
            if (Player* player = dynamic_cast<Player*>(entity)) {
                if (player->getID() != mPlayerID) {
                    sf::Color color = sprite.getColor();
                    color.a = 128;
                    sprite.setColor(color);
                }
            }
            mWindow.draw(sprite);
        }
    }
    mWindow.display();
}
