/*
** server.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 19:33:46 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 23:03:48 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "server.hpp"

Server::Server() 
{
};

Server::~Server()
{
};

void Server::init_server(int ac, char **av)
{
    if (ac < 3)
        throw std::runtime_error("Usage : ./jetpack_server -p <port> -m <map> [-d]");
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSocket < 0)
        throw std::runtime_error("Init error : Socket");
    
    mServerAddressControl.sin_family = AF_INET;
    mServerAddressControl.sin_port = htons(std::atoi(av[1]));
    mServerAddressControl.sin_addr.s_addr = INADDR_ANY;

    int on = 1;
    if (setsockopt(mServerSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
        throw std::runtime_error("Error with setsockopt");
        
    if (bind(mServerSocket, (struct sockaddr*)&mServerAddressControl, sizeof(mServerAddressControl)) < 0)
        throw std::runtime_error("Init error : bind");

    if (listen(mServerSocket, 5) < 0)
        throw std::runtime_error("Error with listen");
        
    mPoll.push_back({
        .fd = mServerSocket,
        .events = POLLIN
    });

    mPlayerManager = mPlayerManager->getInstance();
    mRooms.push_back(new NetworkSalon("Default"));
    mRooms.push_back(new NetworkSalon("Salon 1"));
    mRooms.push_back(new NetworkSalon("Salon 2"));
    mRooms.push_back(new NetworkSalon("Salon 3"));
    mRooms.push_back(new NetworkSalon("Salon 4"));
    mRooms.push_back(new NetworkSalon("Salon 5"));
}

//https://stackoverflow.com/questions/71572056/multithreaded-server-c-socket-programming
// POS x y              -> Update position    - Client Send
// DEC id               -> Player disconnect  - Client Send
// MOV id x y           -> Update players pos - Server Send
// JON id name          -> Player Join        - Server Send
// NBP nb               -> Nb players         - Server Send
// DED id               -> Player who dead    - Server Send
// WIN id               -> Player who win     - Server Send
// PAU                  -> Pause              - Server / Client Send
// EPU                  -> End Pause          - Client Send
// RET                  -> Restart Game       - Server Send
// SNA name             -> Set Username       - Client Send
// --------------------Startup Client ----------------------
// OBS nb_obstacles         -> envoi de ça pour dire combien de fois listen
// CON nb_coins             -> mm chose
// POB pos_x_obs pos_y_obs  -> for nb_obstacles send POB[i]
// PCO pos_x_con pos_y_con  -> mm chose
void Server::handlePlayerCommands(Player *player)
{
    char memory[1024] = { 0 };
    std::string command;
    std::smatch m;
    int n;
    
    while ((n = recv(player->getPlayerSocket(), memory, 1024, 0))) {
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
                player->getSalon()->CreateMessage(std::string("MOV " + std::to_string(player->getID()) + " " + std::to_string(player->getPosition().first) + " " + std::to_string(player->getPosition().second)), Type::POSITION, player->getID());
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

// pour poll
// https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select
// multi thread
// https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/
// mutex pour l'accès aux données (createplayer, etc)
// https://bousk.developpez.com/cours/multi-thread-mutex/
void Server::start_server()
{
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