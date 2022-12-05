#include "header.hpp"
#include <libc.h>
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
	private:
		std::vector<Location> locations;
		void parse(std::vector<std::string> config);
		std::string _root;
		std::string _name;
		std::string _host;
		int _port;
		std::vector<std::string> _indexes;
};