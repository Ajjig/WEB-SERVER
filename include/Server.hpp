#pragma once

#include "header.hpp"

class Server {
	public:
		Server() {};
		Server( std::vector<std::string> & config, size_t & i );
		~Server();
		void addLocation(Location location);
		void addIndex(std::string ind);
		void setRoot(std::string root);
		std::string getRoot();
		std::vector<std::string> getIndexes();
		std::vector<Location> getLocations();
		void addName(std::string name);
		std::vector<std::string> getNames();
		void put();
		int getPort();
		std::string getHost();
		std::string getName();
		bool isBind();
		void setBind(bool val);
		void setport(int port);
		void setEnv(char **envp);
		char **getEnv();
		std::string getCGI(std::string ext);
		std::vector<std::string> & getLocationPaths();
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
};

std::ostream & operator<<(std::ostream & out, Server const &server);