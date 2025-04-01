/*
** EPITECH PROJECT, 2025
** client.c
** File description:
** client.c
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <unistd.h>
#include "client.hpp"
#include <regex>
#include <future>
#include <thread>

int init(Client *client, int ac, char **av) {
    struct sockaddr_in addr;
    char data[2048];
    int size = 0;

    client->_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4242);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client->_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw std::runtime_error("Impossible to connect");

    size = read(client->_fd, data, sizeof(data));
    data[size] = '\0';
    
    std::cout << data;
    return atoi(&data[3]);
}

void check_send_pos(Client *client, Game *game) {
    int size = 0;
    char data[2048];

    while (true) {
        size = read(client->_fd, data, sizeof(data));
        if (size <= 0) {
            std::cout << "STOP" << std::endl;
            break;
        }
        std::string data_str(data, size);
        if (data_str.substr(0, 3) == "550") {
            std::cout << "pos requested" << std::endl;
            int my_id = game->_id;
            std::string pos = "200 New Pos (" + std::to_string(game->_players[my_id]._pos.x) + ", " + 
                             std::to_string(game->_players[my_id]._pos.y) + ")";
            std::cout << pos << std::endl;
            write(client->_fd, pos.c_str(), pos.length());
        }
        if (data_str.substr(0, 3) == "250") {
            std::regex const e{"\\((\\d+),\\s*([A-Za-z0-9]+)\\)"};
            std::smatch m;

            if (std::regex_search(data_str, m, e)) {
                int id = std::stoi(m[1].str());
                std::string name = m[2].str();
                std::cout << "New player joined " << id << " - " << name << std::endl;
                game->_players[id] = Player(id, name);
            } else {
                std::cout << "Error - New player not valid" << std::endl;
            }
        }
        if (data_str.substr(0, 3) == "300") {
            std::regex const e{"\\((\\d+),\\s*(\\d+(\\.\\d+)?),\\s*(\\d+(\\.\\d+)?)\\)"};
            std::smatch m;
        
            if (std::regex_search(data_str, m, e)) {
                int player_id = std::stoi(m[1].str());
                float x = std::stof(m[2].str());
                float y = std::stof(m[4].str());
                
                std::cout << data_str << std::endl;

                if (player_id != game->_id) {
                    std::cout << "Update player " << player_id << " position: " << x << ", " << y << std::endl;
                    
                    if (game->_players.find(player_id) != game->_players.end()) {
                        game->_players[player_id]._pos.x = x;
                        game->_players[player_id]._pos.y = y;
                        game->_players[player_id].square.setPosition(game->_players[player_id]._pos);
                    } else {
                        std::cout << "Player " << player_id << " not found, adding to game" << std::endl;
                        game->_players[player_id] = Player(player_id, "Player" + std::to_string(player_id));
                        game->_players[player_id]._pos.x = x;
                        game->_players[player_id]._pos.y = y;
                    }
                }
            } else {
                std::cout << "Error - Invalid player update format: " << data_str << std::endl;
            }
        }
    }
}

int display(Client *client, Game *game) {
    int size = 0;
    char data[2048];

    game->create_window();
    while (game->is_Open()) {
        game->handle_events();
        game->draw();
    }
    return 0;
};

int main(int ac, char **av) {
    Client client;
    Game game;
    int id = 0;
    
    try {
        id = init(&client, ac, av);
        game.init(id);
        std::thread t1(check_send_pos, &client, &game);
        t1.detach();
        display(&client, &game);
        t1.join();
        return 0;
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return 84;
    }
}