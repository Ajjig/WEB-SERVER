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
OBJDIR = ./.build

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.cpp $(INC)
	@mkdir -p $(dir $@)

	@printf "\033[1;31m[ ◯ ] $< ...\033[0m %s"
	@$(CC) $(CPPFLAGS) -c $< -o $@
	@printf "\033[0K\r\033[32m[ ✔ ]    $<\033[0m %s\n"

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

all: $(NAME)

.PHONY: all clean fclean re

.DEFAULT_GOAL = all