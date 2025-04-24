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
#include "graphical_assets/Rect.hpp"
#include "graphical_assets/Text.hpp"
#include "graphical_assets/Bouton.hpp"

#define FLOOR 480
#define CEILING 0

enum LobbyMenu {
    BASE = -1,
    CREATE_SALON = 0,
    LIST_SALON = 1,
};

enum GameMode {
    START = -1,
    LOBBY = 0,
    GAME = 1
};

struct Rooms {
    std::string name;
    int max_nb_players;
    int active_nb_players;
};

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
    protected:
        void create_window(void);
        void handle_events(void);
        void handle_menu_events(void);
        void move_background(void);
        void handleAnimations(void);
        void draw(void);
        void MenuDraw(void);
        void GameDraw(std::vector<IEntity*> entities);
        void StartDraw(void);
        void DrawRect(Rect rect);
        void DrawText(Text text);
        void DrawButon(Bouton *buton);
        void HandleClick(void);
        void DisplayNewSalon(Rooms room);
    private:
        /* DATA */
        int mPort;
        int mPlayerID;
        float mMapWidth;
        bool mGameReady;
        bool mPosThread;
        bool mDebugMode;
        float mMapHeight;
        int mPlayerSocket;
        bool mHasUsername;
        float mScaleFactor;
        std::string mIp;
        std::vector<Coin*> mCoins;
        std::string mPlayerUsername;
        std::atomic<bool> mRunning{true};
        struct sockaddr_in mAddressControl;
        std::vector<std::thread> mPoolThread;
        std::vector<ElectricBarrier*> mBarriers;
        std::map<std::string, Rooms> mActiveRooms;
        std::vector<std::unique_ptr<Bouton>> mStartButtons;
        std::vector<std::unique_ptr<Bouton>> mCreateButtons;
        std::vector<std::unique_ptr<Bouton>> mJoinButtons;

        bool mAlreadyJoining;
        std::string mCreateServerName;
        int mCreateServerPort;

        /* SFML */
        sf::Font mFont;
        sf::View mView;
        sf::Event mEvent;
        sf::String mInput;
        sf::VideoMode mMode;
        sf::Vector2f mViewPos;
        sf::Texture mBackground;
        sf::RenderWindow mWindow;
        sf::String mInputServerName;
        sf::Text mServerNameDisplay;
        sf::Text mPlayerInputDisplay;
        sf::Text mServerNbPlayersDisplay;
        
        /* Managers */
        SoundManager mSoundManager;
        PlayersManager *mPlayerManager;

        /* Modes */
        GameMode mGameMode;
        LobbyMenu mLobbyMenu;
};

#endif
