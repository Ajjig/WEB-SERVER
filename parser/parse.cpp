#include "../include/header.hpp"

void split(string & buff, std::vector<string> & config) {
	for (size_t i = 0; i < buff.size(); i++) { // find "{;}"
		if (buff[i] == ';' || buff[i] == '{' || buff[i] == '}') {
			buff.insert(i, " ");
			buff.insert(i + 2, " ");
			i += 2;
		}
	}

	for (size_t i = 0; i < buff.size(); i++) {
		if (buff[i] != ' ') {
			config.push_back(buff.substr(i, buff.find(' ', i) - i));
			i = buff.find(' ', i);
		}

	}
}

void validate(std::vector<string> & config) {
	std::stack <string> stack;
	for (size_t i = 0; i < config.size(); i++) {
		if (config[i] == "{") {
			stack.push("{");
		}
		else if (config[i] == "}") {
			if (stack.empty()) {
				std::cerr << "Error: invalid config ( missing '{' )" << std::endl;
				exit(EXIT_FAILURE);
			}
			else {
				stack.pop();
			}
		}
	}
	if (!stack.empty()) {
		std::cerr << "Error: invalid config ( missing '}' )" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	bind_checker(std::vector<Server> & servers, Server & s) {
	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i].getHost() == s.getHost() && servers[i].getPort() == s.getPort()) {
			s.setBind(false);
			return ;
		}
	}
	s.setBind(true);
}

std::vector<Server> parse( int ac, char ** av , char ** envp) {

	std::fstream		file;
	string				buff;
	string				line;
	std::vector<string>	config;
	std::vector<Server>	servers;


	if (ac == 1)
		file.open("./webserv.conf", std::ios::in);
	else
		file.open(av[1], std::ios::in);
	if (!file.is_open()) {
		std::cout << "Error: can't open the config file" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (std::getline(file, line)) {
		if (line.find('#') != std::string::npos)
			line = line.substr(0, line.find('#'));
		buff += line + ";";
	}
	file.close();
	split(buff, config); // split the config file into tokens
	validate(config); // validate the config file
	if (std::find(config.begin(), config.end(), "server") == config.end()) {
		std::cout << "Error: invalid config ( missing 'server' )" << std::endl;
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < config.size(); i++) {
		if (config[i] == "server") {
			Server s(config, i);
			bind_checker(servers, s);
			s.setEnv(envp);
			servers.push_back(s);
		}
	}
	return servers;
}

