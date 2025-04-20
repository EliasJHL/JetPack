/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** INetworkObserver
*/

#ifndef INETWORKOBSERVER_HPP_
#define INETWORKOBSERVER_HPP_

#include <string>

enum Type {
    MOVE = 1,
    CONNECT = 2,
    DISCONNECT = 3,
    DIE = 4,
    WIN = 5,
    PAUSE = 6,
    RESTART = 7,
    NB_PLAYERS = 8,
    POSITION = 9,
    START = 10,
    COIN = 11,
    OBSTACLE = 12
};

struct Message {
    Type type;
    unsigned int PlayerID;
    std::string message;
};

class INetworkObserver {
    public:
        virtual ~INetworkObserver() {};
        virtual void Update(const Message &message) = 0;
};

#endif /* !INETWORKOBSERVER_HPP_ */
