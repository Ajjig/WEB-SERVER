#include "../../include/header.hpp"

Location::Location(std::vector<string> & config, size_t & i) {
	_root = "";
	_autoindex = false;
	_isRedirect = false;
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

bool Location::isRedirect() {
	return _isRedirect;
}

int Location::getRedirectCode() {
	return _redirectCode;
}

std::string Location::getRedirectUrl() {
	return _redirectUrl;
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
			while (config[++i] != ";") {
				addIndex(config[i]);
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
			break ;
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
		else if (config[i] == "redirect") {
			if (atoi(config[++i].c_str()) <= 0) {
				std::cout << "Error: redirect code must be a positive integer" << std::endl;
				exit(EXIT_FAILURE);
			}
			_redirectCode = atoi(config[i].c_str());
			if (config[++i] != ";") {
				_redirectUrl = config[i];
				_isRedirect = true;
			}
			else {
				std::cout << "Error: redirect must be followed by a code and a url" << std::endl;
				exit(EXIT_FAILURE);
			}

		}
	}
	if (not _root.size()) {
		std::cerr << "Error: root path must be specified" << std::endl;
		exit(EXIT_FAILURE);
	}
}