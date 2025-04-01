/*
** GameManager.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:37:24 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:02:51 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef GAMEMANAGER_HPP_
# define GAMEMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.hpp"
#include "PlayersManager.hpp"

class GameManager {
    public:
        GameManager();
        ~GameManager();
        void init_game(int ac, char **av);
        void run_game(void);
        void close_connection(void);
    private:
        void create_window(void);
        void handle_events(void);
        void handle_server(void);
        void draw(void);
        sf::RenderWindow mWindow;
        sf::Event mEvent;
        int mPlayerID; // Que pour le client sur le jeu (pas les autres en réseau)
        int mPlayerSocket;
};

#endif /* !GAMEMANAGER_HPP_ */
