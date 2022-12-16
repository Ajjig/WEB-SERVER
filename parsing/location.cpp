#include "../include/header.hpp"

Location::Location(std::vector<string> & config, size_t & i) {
	_root = "./";
	_indexes.push_back("index.html");
	parse(config, i);
}

Location::~Location() {}

std::vector<string> Location::getAllowed() {
	return _allowed;
}

bool Location::isAllowed(std::string method) {
	return (std::find(_allowed.begin(), _allowed.end(), method) != _allowed.end());
}

void Location::setAllowed(std::string method) {
	_allowed.push_back(method);
}

void Location::setRoot(string root) {
	_root = root;
}

string Location::getRoot() {
	return _root;
}

void Location::addIndex(string index) {
	_indexes.push_back(index);
}


bool Location::isAutoindex() {
	return _autoindex;
}

std::vector<string> Location::getIndexes() {
	return _indexes;
}

void Location::parse(std::vector<string> & config, size_t & i) {

	for ( ; i < config.size(); i++) {
		if (config[i] == "root") {
			if (config[++i] == ";") {
				std::cout << "Error: root must be followed by a path" << std::endl;
				exit(EXIT_FAILURE);
			}
			setRoot(config[i]);
		}
		else if (config[i] == "index") {
			while (config[i] != ";") {
				addIndex(config[i + 1]);
				i++;
			}
		}
		else if (config[i] == "allow") {
			while (config[++i] != ";") {
				if (METHODS.find(config[i]) > METHODS.size()) {
					std::cerr << "Error: method '" << config[i] << "' is unknown" << std::endl;
					exit(EXIT_FAILURE);
				}
				setAllowed(config[i]);
			}
		}
		else if (config[i] == "}") {
			return ;
		}
		else if (config[i] == "autoindex") {
			if (config[++i] == "on")
				_autoindex = true;
			else if (config[i] == "off")
				_autoindex = false;
			else {
				std::cout << "Error: autoindex must be followed by 'on' or 'off'" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}