#include "include/header.hpp"
#include "include/socket.hpp"
int main( int ac, char ** av ) {

	Server server = parse(ac, av);
	server.put(); // puts the server information
	
	// sockets
	Socket socket(80, "0.0.0.0");
	socket.start();
	return 0;
}