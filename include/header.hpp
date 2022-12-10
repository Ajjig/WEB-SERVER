#ifndef __WEB_SERVER_HPP__
# define __WEB_SERVER_HPP__

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <iterator>
# include "location.hpp"
# include "Server.hpp"
# include "../socket/socket.hpp"
# include <stack>
#include <iostream>
#include <utility>

#include <sstream>
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
#include "../srcs/request/req.hpp"
#include "../srcs/cgi/cgi.hpp"


# define METHODS std::string("GET POST PUT DELETE HEAD OPTIONS TRACE CONNECT PATCH")

typedef std::string string;


Server	parse( int ac, char ** av );

#endif