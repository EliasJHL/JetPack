/*
** Player.hpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/client/include
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Apr 1 20:33:17 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Fri Apr 3 11:23:52 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef PLAYER_HPP_
# define PLAYER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include "SpriteAnimator.hpp"
#include "interfaces/IEntity.hpp"
#include "Clock.hpp"

class Player : public IEntity {
    public:
        Player(int id, std::string name);
        ~Player();
        
        int getID(void) const;
        std::string getName(void) const;
        void setName(std::string name);
        
        void setPosition(std::pair<float, float> pos) override;
        std::pair<float, float> getPosition(void) const override;
        void updateAnimation() override;
        sf::Sprite &getSprite(void) override;
        void setAction(int action, int mode);
        void updateOnlinePlayersPosition(std::pair<float, float> pos);

        void updateScoreText();
        sf::Text &getScoreText();
        void setScore(std::string score);
        std::string getScore() const;

        void setDead(void);
        bool isDead(void);

        void setWin(void);
        bool isWin(void);

    private:
        int mPlayerID;
        sf::Sprite mSprite;
        sf::Vector2f mPos;
        sf::Texture mTexture;
        std::string mPlayerName;
        std::string mScore;
        sf::Text mScoreText;
        sf::Font mFont; 
        SpriteAnimator mAnimator;
        Clock mAnimationTimer;
        bool mIsDead;
        bool mWin;
};

#endif /* !PLAYER_HPP_ */