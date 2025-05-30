##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile
##

NAME_SERVER	=	jetpack_server
NAME_CLIENT	=	jetpack_client

SRC_SERVER	=	server/src/*.cpp server/include/*.cpp
SRC_CLIENT	=	client/src/*.cpp client/include/*.cpp

SFML_FLAGS	=	$(shell pkg-config --libs sfml-all)
SFML_CFLAGS	=	$(shell pkg-config --cflags sfml-all)

all:	server	client

$(NAME_CLIENT): $(SRC_CLIENT)
	g++ -o $(NAME_CLIENT) $(SRC_CLIENT) $(SFML_CFLAGS) -I \
	client/include/ $(SFML_FLAGS)
	@echo "\033[32m= = = = =  COMPILATION COMPLETED  = = = = =\033[0m"

$(NAME_SERVER): $(SRC_SERVER)
	g++ -o $(NAME_SERVER) $(SRC_SERVER) -I server/include/
	@echo "\033[32m= = = = =  COMPILATION COMPLETED  = = = = =\033[0m"

client: $(NAME_CLIENT)

server: $(NAME_SERVER)

clean:
	@$(RM) $(NAME_SERVER)
	@$(RM) $(NAME_CLIENT)

fclean: clean
	@$(RM) *~
	@echo "\033[31m= = = = = =   CLEAR COMPLETED   = = = = = =\033[0m"

re: fclean all
