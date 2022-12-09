#include "../include/header.hpp"


Server::Server( std::vector<string> config ) {
	_name = "webserv";
	_port = 80;
	_root = "./";
	_host = "None";
	parse(config);
}

Server::~Server() {}

void Server::addLocation(Location location) {
	_locations.push_back(location);
}

void Server::addIndex(string ind) {
	_indexes.push_back(ind);
}

void Server::setRoot(string root) {
	_root = root;
}

string Server::getRoot() {
	return _root;
}

std::vector<string> Server::getIndexes() {
	return _indexes;
}

std::vector<Location> Server::getLocations() {
	return _locations;
}

void Server::parse(std::vector<string> config) {

	int bracket = 0;

	for (size_t i = 0; i < config.size(); i++) {
		if (config[i] == "index")
			while (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
				addIndex(config[i + 1]);
				i++;
			}
		else if (config[i] == "root") {
			if (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
				std::cout << "Error: root must be followed by a path" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		else if (config[i] == "listen") {
			if (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
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
			if (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
				std::cout << "Error: host must be followed by a name" << std::endl;
				exit(EXIT_FAILURE);
			}
			_host = config[i + 1];
		}
		else if (config[i] == "server_name") {
			if (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
				std::cout << "Error: name must be followed by a name" << std::endl;
				exit(EXIT_FAILURE);
			}
			_name = config[i + 1];
		}
		else if (config[i] == "location") {
			if (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
				std::cout << "Error: location must be followed by a path" << std::endl;
				exit(EXIT_FAILURE);
			}
			_locationPaths.push_back(config[i + 1]);
			_locations.push_back(Location(config, ++i));
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

void Server::put( void ) {
	std::cout << "port:     " << _port << std::endl;
	std::cout << "name:     " << _name << std::endl;
	for (size_t i = 0; i < _indexes.size(); i++)
		std::cout << "index[0]: " << _indexes[i] << std::endl;
	std::cout << "host:     " << _host << std::endl;
	std::cout << "root:     " << _root << std::endl;
	std::cout << "LOCATIONS:" << std::endl;
	for (size_t i = 0; i < _locations.size(); i++) {
		std::cout << "Locaton '" << _locationPaths[i] << "' :" << std::endl;
		std::cout << "   Root: " <<  _locations[i].getRoot() << std::endl;
		std::cout << "   Indexes: ";
		for (size_t j = 0; j < _locations[i].getIndexes().size(); j++)
			std::cout << _locations[i].getIndexes()[j] << ", ";
		std::cout << std::endl;
		std::cout << "Allowed methods :" << std::endl << "   ";
		for (size_t j = 0; j < _locations[i].getAllowed().size(); j++)
			std::cout << _locations[i].getAllowed()[j] << ", ";
		std::cout << std::endl;
		std::cout << std::endl;
	}
}