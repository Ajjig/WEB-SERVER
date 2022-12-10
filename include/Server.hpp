#pragma once

#include "header.hpp"

class Server {
	public:
		Server( std::vector<std::string> config );
		~Server();
		void addLocation(Location location);
		void addIndex(std::string ind);
		void setRoot(std::string root);
		std::string getRoot();
		std::vector<std::string> getIndexes();
		std::vector<Location> getLocations();
		void put();
		int getPort();
		std::string getHost();
	private:
		std::vector<Location> _locations;
		std::vector<std::string> _locationPaths;

		void parse(std::vector<std::string> config);
		std::string _root;
		std::string _name;
		std::string _host;
		int _port;
		std::vector<std::string> _indexes;
};

std::ostream & operator<<(std::ostream & out, Server const &server);