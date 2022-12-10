#include "include/header.hpp"
#include "include/socket.hpp"
int main( int ac, char ** av ) {

	Server server = parse(ac, av);
	// server.put(); // puts the server information

	Socket socket(server);
	socket.start();

	return 0;
}