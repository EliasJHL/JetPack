/*
** Server.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 14:00:10 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Thu Mar 26 00:09:56 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef SERVER_HPP_
# define SERVER_HPP_
#include "Network.hpp"
#include "Player.hpp"
#include "PlayersManager.hpp"
#include "Encapsulation.hpp"
#include <thread>
#include <string>
#include <iostream>
#include <regex>

class Server {
    public:
        Server();
        ~Server();
        
        void init_server(int ac, char **av);
        void start_server();
        
    protected:
        void handlePlayerCommands(Player *player);
    private:
        int mServerSocket;
        bool mDebugMode;
        std::string mMapContent;
        int mPort;
        std::vector<NetworkSalon *> mRooms;
        PlayersManager *mPlayerManager;
        struct sockaddr_in mServerAddressControl;
        struct sockaddr_in mClientAddr;
        std::vector<struct pollfd> mPoll;
        std::vector<std::thread> mPoolThread;
};

#endif /* !SERVER_HPP_ */
