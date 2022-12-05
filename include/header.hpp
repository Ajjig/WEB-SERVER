#ifndef __WEB_SERVER_HPP__
# define __WEB_SERVER_HPP__

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <iterator>
# define str std::string
# include "location.hpp"
# include "server.hpp"

Server	parse( int ac, char ** av );

#endif