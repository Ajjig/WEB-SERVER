#ifndef __WEB_SERVER_HPP__
# define __WEB_SERVER_HPP__

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <iterator>
# include "location.hpp"
# include "Server.hpp"
# include "socket.hpp"
# include <stack>

#include <sstream>
#include <libc.h>

Server	parse( int ac, char ** av );

#endif