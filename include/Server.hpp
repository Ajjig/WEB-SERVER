#include "header.hpp"

class Server {
	public:
		Server( std::vector<str> config );
		~Server();
		void addLocation(Location location);
		void addIndex(str ind);
		void setRoot(str root);
		str getRoot();
		std::vector<str> getIndexes();
		std::vector<Location> getLocations();
	private:
		std::vector<Location> locations;
		void parse(std::vector<str> config);
		str _root;
		str _name;
		str _host;
		int _port;
		std::vector<str> _indexes;
};