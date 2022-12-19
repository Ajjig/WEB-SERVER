#pragma once

# include <fstream>
# include <iterator>
# include <stack>
#include <iostream>
#include <utility>
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <sys/wait.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <map>

#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>
#include <map>
#include <algorithm>
# include <dirent.h>

# include "../parsing/location/location.hpp"
# include "../parsing/server/server.hpp"
# include "../socket/socket.hpp"
#include "../srcs/dirent/dir.hpp"
#include "../srcs/request/req.hpp"
#include "../srcs/respond/res.hpp"
#include "../srcs/cgi/cgi.hpp"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define NC "\033[1;0m"

# define METHODS std::string("GET POST PUT DELETE HEAD OPTIONS TRACE CONNECT PATCH")

typedef std::string string;

std::vector<Server>	parse( int ac, char ** av , char ** envp);
