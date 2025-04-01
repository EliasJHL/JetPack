/*
** Player.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:33:17 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:09:47 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef PLAYER_HPP_
# define PLAYER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>

class Player {
    public:
        Player(int id, std::string &name);
        ~Player();
        
        int getID(void) const;
        std::string getName(void) const;
        std::pair<float, float> getPosition(void) const;
        sf::Sprite getPlayerSprite(void);
        
        void setPosition(std::pair<float, float> pos);
    private:
        int mPlayerID;
        sf::Sprite mSprite;
        sf::Vector2f mPos;
        std::string mPlayerName;
};

#endif /* !PLAYER_HPP_ */
