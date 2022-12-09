#include "include/header.hpp"

int main( int ac, char ** av ) {

	Server server = parse(ac, av);
	server.put(); // puts the server information
	//Socket socket;
	//socket.start();
	return 0;
}