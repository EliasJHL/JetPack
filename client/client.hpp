/*
** client.hpp for B-NWP-400-MPL-4-1-myftp-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/my_ftp/B-NWP-400-MPL-4-1-myftp-elias-josue.hajjar-llauquen/bonus/client
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Sat Mar 15 02:16:50 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 21:48:37 2025 Elias Josué HAJJAR LLAUQUEN
*/

#ifndef CLIENT_HPP_
# define CLIENT_HPP_

#include <iostream>
#include <regex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <mutex>

class Player {
    public:
        Player(int id, std::string name) {
            _id = id;
            _name = name;
            _pos.x = 350;
            _pos.y = 250;
            square.setSize(sf::Vector2f(100, 100));
            square.setFillColor(sf::Color(255, 0, 0));
            square.setPosition(_pos);
        };
        Player() : _id(0), _name("Unknown") {
            _pos.x = 350;
            _pos.y = 250;
            square.setSize(sf::Vector2f(100, 100));
            square.setFillColor(sf::Color(255, 0, 0));
            square.setPosition(_pos);
        }    
        ~Player() {};
        void update_pos(int x, int y) {
            _pos.x = x;
            _pos.y = y;
        };
        void move_up() {
            _pos.y -= 10;
            square.setPosition(_pos);
        };
        void move_down() {
            _pos.y += 10;
            square.setPosition(_pos);
        };
        void move_left() {
            _pos.x -= 10;
            square.setPosition(_pos);
        };
        void move_right() {
            _pos.x += 10;
            square.setPosition(_pos);
        };
        void draw(sf::RenderWindow *window) {
            window->draw(square);
        }
        sf::Vector2f _pos;
        int _id;
        sf::RectangleShape square;
    private:
        std::string _name;
};

class Game {
    public:
        Game() {};
        ~Game() {};
        void init(int id) {
            _players.insert({id, Player(id, "Player" + std::to_string(id))});
            _id = id;
        };
        bool is_Open() {
            return _window.isOpen();
        };
        void create_window() {
            sf::VideoMode mode(800, 600, 32);
            _window.create(mode, "SFML", sf::Style::Resize | sf::Style::Close);
        };
        void handle_events() {
            while (_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    _window.close();
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {// position x - 10
                    _players[_id].move_left();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D) {// pos x + 10
                    _players[_id].move_right();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {// pos y - 10
                    _players[_id].move_up();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {// pos y + 10
                   _players[_id].move_down();
                }
            }
        };
        void update_players() {
            
        };
        void draw() {
            _window.clear(sf::Color::Black);
            for (auto& pair : _players) {
                pair.second.draw(&_window);
            }
            _window.display();
        }
        std::map<int, Player> _players;
        int _id;
    private:
        sf::RenderWindow _window;
        sf::Event event;
};

class Client {
    public:
        int _fd;
        std::mutex mtx;
};

#endif /* !CLIENT_HPP_ */