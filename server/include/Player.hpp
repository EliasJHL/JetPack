/*
** Player.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 10:56:51 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 16:20:43 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#include <memory>
#include <string>
#include "Network.hpp"

class Player {
    public:
        Player(int id, const std::string &name, int socket);
        ~Player();
        
        unsigned int getID(void) const;
        const std::string &getName(void) const;
        bool isConnected(void) const;
        NetworkObserver *getObserver();
        NetworkSalon *getSalon();
        int getPlayerSocket();

        const std::pair<float, float> &getPosition(void) const;
        void setPosition(std::pair<float, float> pos);
        void setConnection(bool connexion);
        void setSalon(NetworkSalon &salon);
        void setPlayerSocket(int fd);
        
    private:
        int mPlayerID;
        int mPlayerSocket;
        std::string mPlayerName;
        std::pair<float, float> mCoordinates;
        bool mConnected;
        NetworkObserver *mObserver;
};

#endif /* !PLAYER_HPP_ */
