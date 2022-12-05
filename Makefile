NAME = webserv
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp ${addprefix ./parsing/, location.cpp tokens.cpp}
OBJS = $(SRCS:.cpp=.o)
INC = ./include/header.hpp
RM = rm -rf


$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(INC)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re

.DEFAULT_GOAL = all