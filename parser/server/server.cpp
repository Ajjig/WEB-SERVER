#include "../../include/header.hpp"

#define VALIDATE_END(error) if (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) { \
				std::cout << error << std::endl; \
				exit(EXIT_FAILURE); \
			} \


Server::Server( std::vector<string> & config, size_t & i ) {
	_port = -1;
	_root = "./";
	_host = "";
	parse(config, i);
}

bool Server::isBind() {
	return _bind;
}

std::string Server::getErrorPage(int code) {
	if (_errorPages.find(code) != _errorPages.end())
		return _errorPages[code];
	return "None";
}

void Server::setBind(bool val) {
	_bind = val;
}

Server::~Server() {}

void Server::addLocation(Location location) {
	_locations.push_back(location);
}

std::string Server::getCGI(std::string ext) {
	if (_cgi.find(ext) != _cgi.end())
		return _cgi[ext];
	return "None";
}

void Server::addIndex(string ind) {
	_indexes.push_back(ind);
}

std::vector<std::string> & Server::getLocationPaths() {
	return _locationPaths;
}

void Server::setRoot(string root) {
	_root = root;
}

string Server::getRoot() {
	return _root;
}

int Server::getPort() {
	return _port;
}
void Server::setport(int port) {
	this->_port = port;
}

void Server::setEnv(char **envp) {
	this->_env = envp;
}

char **Server::getEnv()
{
	return this->_env;
}
std::string Server::getHost() {
	return _host;
}

std::vector<string> Server::getIndexes() {
	return _indexes;
}

std::vector<Location> Server::getLocations() {
	return _locations;
}

std::string Server::getName() {
	if (_names.size() > 0)
		return _names[0];
	return "";
}

std::vector<std::string> Server::getNames() {
	return _names;
}

size_t Server::getMaxBodySize() {
	return _maxBodySize;
}

void Server::addName(std::string name) {
	_names.push_back(name);
}

void Server::parse(std::vector<string> & config, size_t & i) {

	int		bracket	= 0;
	size_t	tmp = i - 1;
	bool	multiplePorts = false;
	i += 2 ;

	for ( ; i < config.size(); i++) {
		if (config[i] == "index") {
			while (i + 1 >= config.size() || string("{:}").find(config[i + 1]) != string::npos) {
				addIndex(config[i + 1]);
				i++;
			}
		}
		else if (config[i] == "root") {
			VALIDATE_END("Error: root must be followed by a path");
			setRoot(config[i + 1]);
		}
		else if (config[i] == "listen") {
			while (config[++i] == "");
			VALIDATE_END("Error: listen must be followed by a number 0 - 65535");
			_port = std::atoi(config[i].c_str());
			if (std::atoi(config[i + 1].c_str()) != 0) {
				multiplePorts = true;
				config[i] = "";
			}
		}
		else if (config[i] == "host") {
			VALIDATE_END("Error: host must be followed by a name");
			_host = config[i + 1];
		}
		else if (config[i] == "server_name") {
			VALIDATE_END("Error: server_name must be followed by a name");
			addName(config[i + 1]);
		}
		else if (config[i] == "location") {
			VALIDATE_END("Error: location must be followed by a path");
			_locationPaths.push_back(config[i + 1]);
			_locations.push_back(Location(config, ++i));
		}
		else if (config[i] == "cgi") {
			VALIDATE_END("Error: cgi must be followed by an extension");
			i++;
			VALIDATE_END("Error: cgi extension must be followed by a path");
			_cgi[config[i]] = config[i + 1];
		}
		else if (config[i] == "error_page") {
			VALIDATE_END("Error: error_page must be followed by a number");
			i++;
			int err_nbr = atoi(config[i].c_str());
			if (err_nbr < 100 || err_nbr >= 600) {
				std::cerr << "Error: error_page must be followed by correct error number" << std::endl;
				exit(EXIT_FAILURE);
			}
			VALIDATE_END("Error: error_page number must be followed by a path");
			_errorPages[err_nbr] = config[i + 1];
		}
		else if (config[i] == "max_body_size") {
			VALIDATE_END("Error: max_body_size must be followed by a number");
			_maxBodySize = atoi(config[i + 1].c_str());
			if (_maxBodySize < 1024) {
				std::cerr << "Error: max_body_size must be at least 1024 bytes" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		///////
		else if (config[i] == "}")
			bracket--;
		else if (config[i] == "{")
			bracket++;
		if (bracket == -1)
			break;
	}
	if (_port <= 0 || _port > 65535) {
		std::cout << "Error: port must be 0 <= p <= 65535, found (" << _port << ")" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (multiplePorts)
		i = tmp;
}

void Server::put( void ) {
	std::cout << RED << "PORT :    " << NC << _port << std::endl;
	std::cout << RED << "NAMES:    " << NC;
	for (size_t i = 0; i < _names.size(); i++)
		std::cout << _names[i] << " ";
	std::cout << std::endl;
	std::cout << RED << "HOST :    " << NC << _host << std::endl;
	std::cout << RED << "CGI  :" << NC << std::endl;
	for (std::map<std::string, std::string>::iterator it = _cgi.begin(); it != _cgi.end(); it++)
		std::cout << "           " << it->first << GREEN << "  ->  " << NC << it->second << std::endl;
	std::cout << std::endl;
	std::cout << RED << "LOCATIONS:" << NC << std::endl;
	for (size_t i = 0; i < _locations.size(); i++) {
		std::cout << "   Location -> " << YELLOW << _locationPaths[i] << ( _locations[i].isRedirect() \
			? (std::string(GREEN) + " >>> " + std::to_string(_locations[i].getRedirectCode()) \
			+ " " + _locations[i].getRedirectUrl()) : "") << NC << " :" << std::endl;
		std::cout << BLUE << "      Root: " << NC <<  _locations[i].getRoot() << std::endl;
		std::cout << BLUE << "      Indexes: " << NC;
		for (size_t j = 0; j < _locations[i].getIndexes().size(); j++)
			std::cout << _locations[i].getIndexes()[j] << ", ";
		std::cout << std::endl;
		std::cout << BLUE << "      Allowed methods :" << NC << " ";
		for (size_t j = 0; j < _locations[i].getAllowed().size(); j++)
			std::cout << _locations[i].getAllowed()[j] << ", ";
		std::cout << std::endl;
	}
}