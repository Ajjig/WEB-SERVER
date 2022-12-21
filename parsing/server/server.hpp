#pragma once

#include "../../include/header.hpp"

class Server {
	public:
		Server() {};
		Server( std::vector<std::string> & config, size_t & i );
		~Server();
		void addLocation(Location location);
		void addIndex(std::string ind);
		void addName(std::string name);

		void setBind(bool val);
		void setport(int port);
		void setRoot(std::string root);
		void setEnv(char **envp);

		std::string 				getRoot();
		std::vector<std::string>	getIndexes();
		std::vector<Location>		getLocations();
		std::vector<std::string>	getNames();
		std::vector<std::string>	&getLocationPaths();
		int 						getPort();
		std::string 				getCGI(std::string ext);
		std::string 				getHost();
		std::string 				getName();
		size_t 						getMaxBodySize();

		void put();
		bool isBind();
		char **getEnv();
		std::string getErrorPage(int code);

	private:

		std::vector<Location> _locations;
		std::vector<std::string> _locationPaths;
		bool _bind;
		void parse(std::vector<std::string> & config, size_t & i);
		std::string _root;
		std::vector<std::string> _names;
		std::string _host;
		int _port;
		char **_env;
		std::vector<std::string> _indexes;
		std::map<std::string, std::string> _cgi;
		std::map<int, std::string> _errorPages;
		size_t _maxBodySize;
};

std::ostream & operator<<(std::ostream & out, Server const &server);