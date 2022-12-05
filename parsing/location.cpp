#include "../include/header.hpp"

Location::Location(std::vector<std::string> config) {
	parse(config);
}

Location::~Location() {}


void Location::setRoot(std::string root) {
	_root = root;
}

std::string Location::getRoot() {
	return _root;
}

void Location::addIndex(std::string index) {
	_indexes.push_back(index);
}

std::vector<std::string> Location::getIndexes() {
	return _indexes;
}

void Location::parse(std::vector<std::string> config) {

	for (size_t i = 0; i < config.size(); i++) {
		if (config[i] == "root") {
			setRoot(config[i + 1]);
			if (config[++i] == ";") {
				std::cout << "Error: root must be followed by a path" << std::endl;
				break;
			}
		}
		else if (config[i] == "index") {
			while (config[i] != ";") {
				addIndex(config[i + 1]);
				i++;
			}
		}
		else {
			std::cout << "Error: unknown token '"<< config[i] << "' (ignored)" << std::endl;
			while (config[i] != ";")
				i++;
		}
	}
}