#include "../include/header.hpp"


Server::Server( std::vector<str> config ) {
	parse(config);
}

Server::~Server() {}

void Server::addLocation(Location location) {
	locations.push_back(location);
}

void Server::addIndex(str ind) {
	_indexes.push_back(ind);
}

void Server::setRoot(str root) {
	_root = root;
}

str Server::getRoot() {
	return _root;
}

std::vector<str> Server::getIndexes() {
	return _indexes;
}

std::vector<Location> Server::getLocations() {
	return locations;
}

void Server::parse(std::vector<str> config) {

	for (size_t i = 0; i < config.size(); i++) {
		if (config[i] == "index")
			while (i + 1 >= config.size() || str("{:}").find(config[i + 1]) != std::string::npos) {
				addIndex(config[i + 1]);
				i++;
			}
		else if (config[i] == "root") {
			if (i + 1 >= config.size() || str("{:}").find(config[i + 1]) != std::string::npos) {
				std::cout << "Error: root must be followed by a path" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (config[i] == "port") {
			if (i + 1 >= config.size() || str("{:}").find(config[i + 1]) != std::string::npos) {
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
	}
}