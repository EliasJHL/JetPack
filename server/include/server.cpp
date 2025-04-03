/*
** server.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 19:33:46 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 11:25:31 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "server.hpp"
#include <fstream>
#include <cctype>

Server::Server() 
{
};

Server::~Server()
{
};

void Server::init_server(int ac, char **av)
{
    if (ac < 5 || ac > 6)
        throw std::runtime_error("Usage : ./jetpack_server -p <port> -m <map> [-d]");

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
            this->mDebugMode = true;
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
    
    mMapContent.assign((std::istreambuf_iterator<char>(map)),
                       std::istreambuf_iterator<char>());

    
    this->mPort = std::stoi(port);
    if (this->mPort <= 0 || this->mPort > 65535)
        throw std::runtime_error("Invalid port number: " + port);

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

    if (this->mDebugMode) {
        std::cout << "Debug mode enabled." << std::endl;
    }
}

//https://stackoverflow.com/questions/71572056/multithreaded-server-c-socket-programming
// ---------------------Server side-------------------------
// PLR id x y coin      -> Update envoyé      - Server Send
// DEC id               -> Player disconnect  - Server Send
// JON id name          -> Player Connect     - Server Send
// RDY                  -> Player Ready       - Server Send
// SRT                  -> Start Game         - Server Send
// DED id               -> Player who dead    - Server Send
// WIN id               -> Player who win     - Server Send
// PAU id               -> Pause              - Server Send
// RET                  -> Restart Game       - Server Send
// --------------------Startup Client ----------------------
// MAP size_x size_y content map -> Envoyer map - Server Send
// -------------------- Client Send ------------------------
// SNA name             -> Set Username       - Client Send
// PAU                  -> Pause              - Client Send
// EPU                  -> End Pause          - Client Send
// POS x y              -> Envoyer la position- Client Send
// DEC                  -> Player disconnect  - Client Send
void Server::handlePlayerCommands(Player *player)
{
    char memory[1024] = { 0 };
    std::string command;
    std::smatch m;
    int n;
    
    while (true) {
        if (n = recv(player->getPlayerSocket(), memory, 1024, 0)) {
            command = std::string(memory);
            command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());
            command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());
            if (command.substr(0,3) == "PAU") {
                if (player->getSalon() != nullptr)
                    player->getSalon()->CreateMessage("PAUSE", Type::PAUSE, player->getID());
            }
            if (command.substr(0,3) == "EPU") {
                
            }
            if (command.substr(0,3) == "DED") {
                std::regex const e{"^DED\\s+(\\d+)$"};
                if (std::regex_search(command, m, e)) {
                    player->getSalon()->CreateMessage(command, Type::DIE, player->getID());
                }
            }
            if (command.substr(0,3) == "WIN") {
                std::regex const e{"^WIN\\s+(\\d+)$"};
                if (std::regex_search(command, m, e)) {
                    player->getSalon()->CreateMessage(command, Type::WIN, player->getID());
                }
            }
            if (command.substr(0,3) == "SNA") {
                std::regex const e{"^SNA\\s+([A-Za-z0-9]+)$"};
                if (std::regex_search(command, m, e)) {
                    player->setPlayerName(m[1]);
                    player->getSalon()->CreateMessage(std::string("JON " + std::to_string(player->getID()) + " " + m[1].str()), Type::CONNECT, player->getID());
                }
            }
            if (command.substr(0,3) == "POS") {
                std::regex const e{"^POS\\s+(-?\\d+(?:\\.\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?)$"};
                if (std::regex_search(command, m, e)) {
                    player->setPosition(std::pair<float, float> {std::stof(m[1]), std::stof(m[2])});
                }
            }
            if (command.substr(0,3) == "DEC") {
                std::regex const e{"^DEC\\s+(\\d+)$"};
                if (std::regex_search(command, m, e)) {
                    player->getSalon()->CreateMessage(std::string("DEC " + std::to_string(player->getID())), Type::DISCONNECT, player->getID());
                    player->getSalon()->Quit(player->getObserver());
                    mPlayerManager->removePlayer(player->getID());
                    return;
                }
            }
            std::cout << "From Player " << player->getID() << " : " << memory;
        }
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
        for (int i = 0; i < players.size(); i++) {
            x = players[i]->getPosition().first;
            y = players[i]->getPosition().second;
            message = std::string("PLY " + std::to_string(players[i]->getID()) + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(players[i]->getCoins()));
            players[i]->getSalon()->CreateMessage(message, Type::POSITION, players[i]->getID());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// pour poll
// https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select
// multi thread
// https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/
// mutex pour l'accès aux données (createplayer, etc)
// https://bousk.developpez.com/cours/multi-thread-mutex/
void Server::start_server()
{
    std::thread p(&Server::updatePlayersInfo, this);
    p.detach();
    while (true) {
        int rc = poll(mPoll.data(), mPoll.size(), 180000);

        if (rc < 0)
            throw std::runtime_error("loop error : poll");

        if (mPoll[0].revents == POLLIN) {
            socklen_t client_len = sizeof(mClientAddr);
            int new_player_socket = accept(mServerSocket, (struct sockaddr*) &mClientAddr, &client_len);

            if (new_player_socket < 0) {
                std::cerr << "New client connection failed" << std::endl;
                continue;
            }

            mPoll.push_back({
                .fd = new_player_socket
            });
            
            int new_player_id = mPlayerManager->createPlayer("Dummy", new_player_socket);
            std::cout << "Connection from " << inet_ntoa(mClientAddr.sin_addr) << ":" << ntohs(mClientAddr.sin_port) << std::endl;
            write(mPlayerManager->getPlayer(new_player_id)->getPlayerSocket(), "Welcome\r\n", std::string("Welcome\r\n").length());
            std::string message = std::string("IDP " + std::to_string(new_player_id) + "\r\n");
            write(mPlayerManager->getPlayer(new_player_id)->getPlayerSocket(), message.c_str(), message.length());
            mPlayerManager->getPlayer(new_player_id)->setSalon(*mRooms[0]);
            std::thread t(&Server::handlePlayerCommands, this, mPlayerManager->getPlayer(new_player_id));
            t.detach();
            mPoolThread.push_back(std::move(t));
        }
        
        for (int i = 0; i < mPoll.size(); i++) {
            if (mPoll[i].revents == 0)
                continue;
            if (mPoll[i].revents == POLLIN) {
                // à voir si nécessaire
            }
        }
    }
}