#include "include/header.hpp"


void put_config(std::vector<Server> & servers) {
	for (size_t i = 0; i < servers.size(); i++) {
		std::cout << "******************** Server " << i + 1 << " ********************" << std::endl;
		servers[i].put();
		std::cout << "**************************************************" << std::endl;
	}
}

int main( int ac, char ** av ) {

	std::vector<Server> servers = parse(ac, av);

	//put_config(servers); /* printing config */

	Socket socket(servers);
	socket.start();

	return 0;
}