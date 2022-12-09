#include "include/header.hpp"
#include "include/socket.hpp"
int main( int ac, char ** av ) {

	// Server server = parse(ac, av);
	// server.put(); // puts the server information
	
	// sockets
	std::map<std::string, std::string> interface_list;
	interface_list.insert(std::pair<std::string,  std::string> ("80", "0.0.0.0"));
	interface_list.insert(std::pair<std::string,  std::string> ("8080", "0.0.0.0"));

	Socket socket(interface_list);
	socket.start();
	return 0;
}