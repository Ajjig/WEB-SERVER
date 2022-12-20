NAME = webserv
CC = c++
CPPFLAGS = -std=c++98 -Wall -Wextra -Werror

SRCS = main.cpp ${addprefix ./parsing/, location/location.cpp parse.cpp server/server.cpp}
SRCS += ${addprefix ./socket/, socket.cpp} \
		${addprefix ./srcs/request/, req.cpp} \
		${addprefix ./srcs/cgi/, cgi.cpp} \
		${addprefix ./srcs/respond/, res.cpp} \
		${addprefix ./srcs/dirent/, dir.cpp}

INC = ./include/header.hpp
RM = rm -rf
OBJDIR = ./objs

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.cpp $(INC)
	@mkdir -p $(dir $@)

	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re

.DEFAULT_GOAL = all