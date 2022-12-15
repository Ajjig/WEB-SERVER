NAME = webserv
CC = c++
CPPFLAGS = -std=c++14

SRCS = main.cpp ${addprefix ./parsing/, location.cpp tokens.cpp server.cpp}
SRCS += ${addprefix ./socket/, socket.cpp} \
		${addprefix ./srcs/request/, req.cpp} \
		${addprefix ./srcs/cgi/, cgi.cpp} \
		${addprefix ./srcs/respond/, res.cpp} \
		${addprefix ./srcs/dirent/, dir.cpp}

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