/*
** Network.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 14:00:39 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 15:17:40 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef NETWORK_HPP_
# define NETWORK_HPP_
#include <string>
#include <list>
#include <iostream>
#include <unistd.h>

enum Type {
    MOVE = 1,
    CONNECT = 2,
    DISCONNECT = 3,
    DIE = 4,
    WIN = 5,
    PAUSE = 6,
    RESTART = 7,
    NB_PLAYERS = 8,
    POSITION = 9
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

class INetwork {
    public:
        virtual ~INetwork(){};
        virtual void Join(INetworkObserver *obs) = 0;
        virtual void Quit(INetworkObserver *obs) = 0;
        virtual void Update() = 0;
};

class NetworkSalon : public INetwork {
    public:
        NetworkSalon(const std::string &name) : mSalonName(name) {};
        virtual ~NetworkSalon() noexcept {
            // debug add message
        };
        
        void Join(INetworkObserver *obs) override {
            std::cout << "New player joined" << std::endl;
            mListObserver.push_back(obs);
        };

        void Quit(INetworkObserver *obs) override {
            mListObserver.remove(obs);
        };
        
        void Update() override {
            std::list<INetworkObserver *>::iterator it = mListObserver.begin();
            while (it != mListObserver.end()) {
                if (*it != nullptr) {
                    (*it)->Update(mMessage);
                }
                ++it;
            }
        };

        void CreateMessage(const std::string &message, Type type, int id) {
            mMessage.message = message;
            mMessage.PlayerID = id;
            mMessage.type = type;
            Update();
        };

        int HowManyPlayers() {
            return mListObserver.size();
        }

        std::string getSalonName() {
            return mSalonName;
        }

    protected:
    private:
        std::list<INetworkObserver *> mListObserver;
        Message mMessage;
        std::string mSalonName;
};

class NetworkObserver : public INetworkObserver {
    public:
        NetworkObserver(NetworkSalon &salon, int PlayerSocket) : mNetworkSalon(salon) {
            this->mNetworkSalon.Join(this);
            this->mNumber = mNumber++;
            this->mSocket = mSocket;
            std::cout << "Hi new player, welcome into salon " << salon.getSalonName() << " \n";
        };
        virtual ~NetworkObserver() {
            std::cout << "Goodbye, I was the Observer \"" << this->mNumber << "\".\n";
        };
        void Update(const Message &message) override {
            mMessageFromSubject = message;
            write(mSocket, message.message.c_str(), message.message.length());
            std::cout << "New message : " << message.message << std::endl;
        };
        void RemoveMe() {
            mNetworkSalon.Quit(this);
        };
        NetworkSalon *getNetworkSalon() {
            return &mNetworkSalon;
        }
    private:
        Message mMessageFromSubject;
        NetworkSalon &mNetworkSalon;
        static int mNumber;
        int mSocket;
};

#endif /* !NETWORK_HPP_ */
