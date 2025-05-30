/*
** GameManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:37:24 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 15:02:16 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef GAMEMANAGER_HPP_
# define GAMEMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <string.h>
#include <regex>
#include <atomic>
#include <poll.h>
#include <sstream>
#include <iomanip>
#include "PlayersManager.hpp"
#include "Coin.hpp"
#include "ElectricBarrier.hpp"
#include "SoundManager.hpp"

#define FLOOR 480
#define CEILING 0

class GameManager {
    public:
        GameManager();
        ~GameManager() {
            std::string m = "DEC\r\n";
            send(mPlayerSocket, m.c_str(), m.length(), 0);
        };
        void init_game(int ac, char **av);
        void commandsHandler(void);
        void posSender(void);
        void run_game(void);
        void close_connection(void);
        void dummyPoll(void);
    protected:
        void create_window(void);
        void handle_events(void);
        void handle_server(void);
        void move_background(void);
        void handleAnimations(void);
        void draw(void);
    private:
        sf::RenderWindow mWindow;
        sf::Event mEvent;
        int mPlayerID; // Que pour le client sur le jeu (pas les autres en réseau)
        int mPlayerSocket;
        std::string mPlayerUsername;
        struct sockaddr_in mAddressControl;
        PlayersManager *mPlayerManager;
        float mMapHeight;
        float mMapWidth;
        float mScaleFactor;
        std::vector<Coin*> mCoins;
        std::vector<ElectricBarrier*> mBarriers;
        bool mHasUsername;
        bool mGameReady;
        bool mDebugMode;
        std::string mIp;
        int mPort;
        bool mRunning;
        sf::Font mFont;
        sf::Texture mBackground;
        sf::String mInput;
        sf::View mView;
        sf::VideoMode mMode;
        sf::Text mPlayerInputDisplay;
        std::vector<std::thread> mPoolThread;
        SoundManager mSoundManager;
        bool mPosThread;
        sf::Vector2f mViewPos;
};

#endif /* !GAMEMANAGER_HPP_ */
