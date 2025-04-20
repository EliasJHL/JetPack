/*
** server.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 19:33:46 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:01:50 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "server.hpp"
#include <fstream>
#include <cctype>
#include <sstream>
#include <iomanip>

Server::Server() 
{
};

Server::~Server()
{
};

void Server::init_server(int ac, char **av)
{
    mPort = parseArguments(ac, av);

    parseMap();
    
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSocket < 0)
        throw std::runtime_error("Init error : Socket");

    mServerAddressControl.sin_family = AF_INET;
    mServerAddressControl.sin_port = htons(this->mPort);
    mServerAddressControl.sin_addr.s_addr = INADDR_ANY;

    int on = 1;
    if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
        throw std::runtime_error("Error with setsockopt");

    if (bind(mServerSocket, (struct sockaddr *)&mServerAddressControl, sizeof(mServerAddressControl)) < 0)
        throw std::runtime_error("Init error : bind");

    if (listen(mServerSocket, 5) < 0)
        throw std::runtime_error("Error with listen");

    mPoll.push_back({
        .fd = mServerSocket,
        .events = POLLIN
    });

    mPlayerManager = mPlayerManager->getInstance();
    mRooms.push_back(new NetworkSalon("Default"));

    if (mDebugMode) {
        std::cout << "Debug mode enabled." << std::endl;
    }
}

int Server::parseArguments(int ac, char **av)
{
    std::string port;
    std::string map_file;

    for (int i = 1; i < ac; ++i) {
        if (std::string(av[i]) == "-p" && i + 1 < ac) {
            port = av[++i];
            if (port.empty() || !std::all_of(port.begin(), port.end(), ::isdigit))
                throw std::runtime_error("Invalid port number: " + port);
        } else if (std::string(av[i]) == "-m" && i + 1 < ac) {
            map_file = av[++i];
        } else if (std::string(av[i]) == "-d") {
            mDebugMode = true;
        } else {
            throw std::runtime_error("Invalid arguments. Usage : ./jetpack_server -p <port> -m <map> [-d]");
        }
    }

    if (port.empty())
        throw std::runtime_error("Missing -p <port> argument.");
    if (map_file.empty())
        throw std::runtime_error("Missing -m <map> argument.");

    std::ifstream map(map_file);
    if (!map.good())
        throw std::runtime_error("Map file not found: " + map_file);
    
    mMapContent.assign((std::istreambuf_iterator<char>(map)), std::istreambuf_iterator<char>());
    
    return std::stoi(port);
}

void Server::threadCheckCollisions(void)
{
    std::string message;
    float scale = 600 / mMapHeight;
    float x1, y1, x2, y2;
    float ox1, oy1, ox2, oy2;
    
    while (true) {
        for (Player *player : mPlayerManager->getReadyPlayer()) {
            try {
                x1 = player->getPosition().first;
                y1 = player->getPosition().second;
                x2 = player->getPosition().first + 134.5;
                y2 = player->getPosition().second + 134.5;

                for (const auto &coin : mCoins) {
                    ox1 = coin.first * scale;
                    oy1 = coin.second * scale;
                    ox2 = ox1 + 171;
                    oy2 = oy1 + 171;
                    if (x1 < ox2 && x2 > ox1 && y1 < oy2 && y2 > oy1) {
                        std::string coinMessage = "COC " + std::to_string(player->getID()) + " " + std::to_string(ox1) + " " + std::to_string(oy1) + "\r\n";
                        player->getSalon()->CreateMessage(coinMessage, Type::COIN, player->getID());
                        player->addCoins(1);
                    }
                }
            
                for (const auto &barrier : mElectricBarriers) {
                    ox1 = barrier.first * scale;
                    oy1 = barrier.second * scale;
                    ox2 = ox1 + 171;
                    oy2 = oy1 + 171;
                    if (x1 < ox2 && x2 > ox1 && y1 < oy2 && y2 > oy1) {
                        std::string deadMessage = "DED " + std::to_string(player->getID());
                        player->getSalon()->CreateMessage(deadMessage, Type::DIE, player->getID()); + "\r\n";
                    }
                }
            } catch (...) {
                continue;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

//https://stackoverflow.com/questions/71572056/multithreaded-server-c-socket-programming
// ---------------------Server side-------------------------
// PLR id x y coin      -> Update envoyé      - Server Send
// DEC id               -> Player disconnect  - Server Send
// JON id name          -> Player Connect     - Server Send
// SRT                  -> Start Game         - Server Send
// DED id               -> Player who dead    - Server Send
// WIN id               -> Player who win     - Server Send
// RET                  -> Restart Game       - Server Send
// COC id x y           -> Collision pièce    - Server Send
// COB id x y           -> Collision avec obstacle - Server Send
// HIH taille           -> Envoi de la hauteur de la map - Server Send when client join
// CON X Y (par rapport à la ligne) -> Envoi de pièces X et Y via X et Y du fichier
// BAR X Y (par rapport à la ligne) -> Envoi des barrières X et Y via X et Y du fichier
// -------------------- Client Send ------------------------
// SNA name             -> Set Username       - Client Send
// POS x y              -> Envoyer la position- Client Send
// DEC                  -> Player disconnect  - Client Send
void Server::handlePlayerCommands(Player *player)
{
    char memory[1024] = { 0 };
    std::string command;
    std::smatch m;
    int n;

    while (true) {
        n = recv(player->getPlayerSocket(), memory, 1024, 0);
        if (n <= 0) {
            std::cout << "Player " << player->getID() << " aka " << player->getName() << " disconnected" << std::endl;
            // if (player->getSalon() != nullptr) {
            //     std::string message = "DEC " + std::to_string(player->getID());
            //     player->getSalon()->CreateMessage(message, Type::DISCONNECT, player->getID());
            // }
            player->ToDelete();
            if (player->getSalon() != nullptr) {
                if (player->getObserver() != nullptr) {
                    player->getSalon()->Quit(player->getObserver());
                }
            }
            close(player->getPlayerSocket());
            mPlayerManager->removePlayer(player->getID());
            std::cout << "Players left : " << mPlayerManager->getAllPlayers().size() << std::endl;
            return;
        }
        memory[n] = '\0';
        command = std::string(memory);
        command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());
        command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());

        Factory CommandFactory;
        IServerCommands *serverCommand = CommandFactory.getCommand(command.substr(0,3));

        if (serverCommand == nullptr)
            continue;
        serverCommand->execute(player->getID(), command);
    }
}

// PLR id x y coin      -> Update envoyé      - Server Send
void Server::updatePlayersInfo()
{
    std::vector<Player*> players;
    std::string message;
    float x, y;
    
    while (true) {
        players = mPlayerManager->getAllPlayers();
        for (Player *player : players) {
            if (player == nullptr || player->getName() == "Dummy" || player->isToDelete())
                continue;
            try {
                if (mPlayerManager->getPlayer(player->getID()) == nullptr)
                    continue;
                x = player->getPosition().first;
                y = player->getPosition().second;
                std::ostringstream oss;
                oss << "PLY " << player->getID() << " " << std::fixed << std::setprecision(2) << x << " " << y << " " << player->getCoins() << "\r\n";
                message = oss.str();
                NetworkSalon* salon = player->getSalon();
                if (salon != nullptr) {
                    salon->CreateMessage(message, Type::POSITION, player->getID());
                }
            } catch (...) {
                continue;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

void Server::sendMapData(int player_socket)
{
    // Envoi de l'hauteur
    std::string heightMessage = "HIH " + std::to_string(mMapHeight) + "\r\n";
    write(player_socket, heightMessage.c_str(), heightMessage.length());

    // Envoi des pièces
    for (const auto &coin : mCoins) {
        std::string coinMessage = "CON " + std::to_string(coin.first) + " " + std::to_string(coin.second) + "\r\n";
        write(player_socket, coinMessage.c_str(), coinMessage.length());
    }

    // Envoi des barrières
    for (const auto &barrier : mElectricBarriers) {
        std::string barrierMessage = "BAR " + std::to_string(barrier.first) + " " + std::to_string(barrier.second) + "\r\n";
        write(player_socket, barrierMessage.c_str(), barrierMessage.length());
    }
}

void Server::initNewPlayer()
{
    int new_player_id;
    socklen_t client_len;

    client_len = sizeof(mClientAddr);
    int new_player_socket = accept(mServerSocket, (struct sockaddr*) &mClientAddr, &client_len);

    if (new_player_socket < 0) {
        std::cerr << "New client connection failed" << std::endl;
        return;
    }

    mPoll.push_back({
        .fd = new_player_socket
    });
    new_player_id = mPlayerManager->createPlayer("Dummy", new_player_socket);
    std::cout << "Connection from " << inet_ntoa(mClientAddr.sin_addr) << ":" << ntohs(mClientAddr.sin_port) << std::endl;
    write(mPlayerManager->getPlayer(new_player_id)->getPlayerSocket(), "Welcome\r\n", std::string("Welcome\r\n").length());
    std::string message = std::string("IDP " + std::to_string(new_player_id) + "\r\n");
    write(mPlayerManager->getPlayer(new_player_id)->getPlayerSocket(), message.c_str(), message.length());
    mPlayerManager->getPlayer(new_player_id)->setSalon(*mRooms[0]);

    sendMapData(new_player_socket);

    std::thread t(&Server::handlePlayerCommands, this, mPlayerManager->getPlayer(new_player_id));
    t.detach();
    mPoolThread.push_back(std::move(t));
};


void Server::start_server()
{
    std::thread updateThread(&Server::updatePlayersInfo, this);
    updateThread.detach();
    std::thread CollisionsThread(&Server::threadCheckCollisions, this);
    CollisionsThread.detach();
    while (true) {
        int rc = poll(mPoll.data(), mPoll.size(), 180000);

        if (rc < 0)
            throw std::runtime_error("loop error : poll");
        
        if (mPoll[0].revents == POLLIN) {
            initNewPlayer();
        }
        
        for (int i = 0; i < mPoll.size(); i++) {
            if (mPoll[i].revents == 0)
                continue;
            if (mPoll[i].revents == POLLIN) {
                // à voir si nécessaire
            }
        }
    }
    if (updateThread.joinable()) {
        updateThread.join();
    }
}

void Server::parseMap() {
    std::istringstream mapStream(mMapContent);
    std::string line;
    int y = 0;

    while (std::getline(mapStream, line)) {
        for (int x = 0; x < line.size(); ++x) {
            if (line[x] == 'c') {
                mCoins.push_back({x, y});
            } else if (line[x] == 'e') {
                mElectricBarriers.push_back({x, y});
            }
        }
        ++y;
    }
    mMapHeight = y;
}