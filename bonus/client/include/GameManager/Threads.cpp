/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** Threads
*/

#include "GameManager.hpp"

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
                        player->setScore(coins, mViewPos);
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
                        mPlayerManager->createPlayer(name, id, mViewPos);
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
                        mViewPos.x = 0;
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
            else if (command.substr(0, 3) == "ROM") {
                std::stringstream messageStream(command);
                std::vector<std::string> parts;
                std::string m;
                
                while (std::getline(messageStream, m, ' ')) {
                    parts.push_back(m);
                }

                if (parts.size() >= 3) {
                    std::string room_name = parts[1];
                    int max_players = std::atoi(parts[2].c_str());
                    int connected_players = std::atoi(parts[3].c_str());

                    mActiveRooms.emplace(room_name, Rooms{room_name, max_players, connected_players});
                }
            }
            else if (command.substr(0, 3) == "JOK") {
                mGameMode = GAME;
                std::cout << "OK SERVER CHANGED" << std::endl;
            }
            else if (command.substr(0, 3) == "JNO") {
                mAlreadyJoining = false;
                std::cout << "Sorry, server already FULL" << std::endl;
            }
            else {
                continue;
            }
        }
    }
}