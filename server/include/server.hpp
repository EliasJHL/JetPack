/*
** Server.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 14:00:10 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:02:45 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef SERVER_HPP_
# define SERVER_HPP_
#include "Player.hpp"
#include "NetworkSalon.hpp"
#include "PlayersManager.hpp"
#include "commands/Factory.hpp"
#include <map>
#include <regex>
#include <thread>
#include <string>
#include <poll.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>

class Server {
    public:
        Server();
        ~Server();
        
        void init_server(int ac, char **av);
        void start_server();
        
    protected:
        void threadCheckCollisions(void);
        void handlePlayerCommands(Player *player);
        void updatePlayersInfo();
        void initNewPlayer();
        void sendMapData(int player_socket);
        int parseArguments(int ac, char **av);
        void parseMap();
    private:
        int mPort;
        int mMapHeight;
        int mMapWidth;
        int mServerSocket;
        bool mDebugMode;
        std::string mMapContent;
        PlayersManager *mPlayerManager;
        struct sockaddr_in mClientAddr;
        struct sockaddr_in mServerAddressControl;
        std::vector<struct pollfd> mPoll;
        std::vector<NetworkSalon *> mRooms;
        std::vector<std::thread> mPoolThread;
        std::vector<std::pair<int, int>> mCoins;
        std::vector<std::pair<int, int>> mElectricBarriers;
};

#endif /* !SERVER_HPP_ */
