#include "../include/header.hpp"


Server::Server( std::vector<std::string> config ) {
	_name = "webserv";
	_port = 80;
	_root = "./";
	_host = "None";
	parse(config);
}

Server::~Server() {}

void Server::addLocation(Location location) {
	locations.push_back(location);
}

void Server::addIndex(std::string ind) {
	_indexes.push_back(ind);
}

void Server::setRoot(std::string root) {
	_root = root;
}

std::string Server::getRoot() {
	return _root;
}

std::vector<std::string> Server::getIndexes() {
	return _indexes;
}

std::vector<Location> Server::getLocations() {
	return locations;
}

void Server::parse(std::vector<std::string> config) {

	int bracket = 0;

	for (size_t i = 0; i < config.size(); i++) {
		if (config[i] == "index")
			while (i + 1 >= config.size() || std::string("{:}").find(config[i + 1]) != std::string::npos) {
				addIndex(config[i + 1]);
				i++;
			}
		else if (config[i] == "root") {
			if (i + 1 >= config.size() || std::string("{:}").find(config[i + 1]) != std::string::npos) {
				std::cout << "Error: root must be followed by a path" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (config[i] == "port") {
			if (i + 1 >= config.size() || std::string("{:}").find(config[i + 1]) != std::string::npos) {
				std::cout << "Error: port must be followed by a number" << std::endl;
				exit(EXIT_FAILURE);
			} else {
				_port = std::stoi(config[i + 1]);
				if (_port < 0 || _port > 65535) {
					std::cout << "Error: port must be 0 < p <= 65535" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}
		else if (config[i] == "host") {
			if (i + 1 >= config.size() || std::string("{:}").find(config[i + 1]) != std::string::npos) {
				std::cout << "Error: host must be followed by a name" << std::endl;
				exit(EXIT_FAILURE);
			}
			_host = config[i + 1];
		}
		else if (config[i] == "server_name") {
			if (i + 1 >= config.size() || std::string("{:}").find(config[i + 1]) != std::string::npos) {
				std::cout << "Error: name must be followed by a name" << std::endl;
				exit(EXIT_FAILURE);
			}
			_name = config[i + 1];
		}
		///////
		else if (config[i] == "}")
			bracket--;
		else if (config[i] == "{")
			bracket++;
		if (bracket == -1)
			break;
	}


}