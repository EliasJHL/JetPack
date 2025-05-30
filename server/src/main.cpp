/*
** main.cpp for B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen in /home/elias/Documents/Epitech/JetPack/B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen/server/src
**
** Made by Elias Josué HAJJAR LLAUQUEN
** Login   <elias-josue.hajjar-llauquen@epitech.eu>
**
** Started on  Tue Mar 25 13:59:21 2025 Elias Josué HAJJAR LLAUQUEN
** Last update Wed Apr 1 20:42:14 2025 Elias Josué HAJJAR LLAUQUEN
*/

#include "server.hpp"

int main(int ac, char **av)
{
    Server serv;

    try {
        if (ac < 5 || ac > 6)
            throw std::runtime_error("Usage : ./jetpack_server -p <port> -m <map> [-d]");
        serv.init_server(ac, av);
        serv.start_server();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
