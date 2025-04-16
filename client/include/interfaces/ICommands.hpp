/*
** EPITECH PROJECT, 2025
** B-NWP-400-MPL-4-1-jetpack-elias-josue.hajjar-llauquen
** File description:
** ICommands
*/

#ifndef ICOMMANDS_HPP_
# define ICOMMANDS_HPP_

#include <vector>
#include <unistd.h>
#include <string>

class ICommands {
    public:
        virtual ~ICommands() = default;
        virtual void execute(std::string arg) = 0;
};

#endif /* !ICOMMANDS_HPP_ */