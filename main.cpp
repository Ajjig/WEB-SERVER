#include "include/header.hpp"

int main( int ac, char ** av ) {

	Server server1 = parse(ac, av);
	Server server2 = parse(ac, av);
	Server server3 = parse(ac, av);
	// server.put(); // puts the server information
	
	std::vector<Server> servers;
	
	server2.setport(8080);
	server3.setport(5850);
	servers.push_back(server1);
	servers.push_back(server2);
	servers.push_back(server3);
	Socket socket(servers);
	socket.start();

	return 0;
}