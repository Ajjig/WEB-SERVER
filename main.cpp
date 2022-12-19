#include "include/header.hpp"
#define USLEEP_TIME 10069

void put_config(std::vector<Server> & servers) {
	for (size_t i = 0; i < servers.size(); i++) {
		std::cout << "******************** Server " << i + 1 << " ********************" << std::endl;
		//servers[i].put();
		std::cout << "        " << servers[i].getHost() << ":" << servers[i].getPort() << " "
			<< ( servers[i].isBind() ? "[X]" : "[ ]" ) << std::endl;
		std::cout << "**************************************************" << std::endl;
	}
}

void put_logs(int ac, char ** av, std::vector<Server> & servers) {
	size_t locationCount = 0;

	for (size_t i = 0; i < servers.size(); i++)
		locationCount += servers[i].getLocations().size();


	std::cout << std::endl << GREEN;
	for (size_t i = 0; i < 87; i++) {
		std::cout << "▉" << std::flush;
		usleep(USLEEP_TIME);
	}
	std::cout << std::endl << std::endl << YELLOW << " * " << "'" << GREEN << ( ac == 1 ? "./webserv.conf" : av[1] ) << YELLOW <<
	"' config file was parsed successfully " <<
	std::endl << " * " <<
	servers.size() << " servers were found" <<
	std::endl << " * " << locationCount << " locations were found" <<
	std::endl << std::endl << NC;
	usleep(USLEEP_TIME);
}


int main( int ac, char ** av, char ** envp)
{

	std::vector<Server> servers = parse(ac, av, envp);
	put_logs(ac, av, servers);

	Socket socket(servers);
	socket.start();

	return 0;
}