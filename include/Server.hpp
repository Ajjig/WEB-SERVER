#include "header.hpp"
#include <libc.h>
class Server {
	public:
		Server( std::vector<string> config );
		~Server();
		void addLocation(Location location);
		void addIndex(string ind);
		void setRoot(string root);
		string getRoot();
		std::vector<string> getIndexes();
		std::vector<Location> getLocations();
	private:
		std::vector<Location> locations;
		void parse(std::vector<string> config);
		string _root;
		string _name;
		string _host;
		int _port;
		std::vector<string> _indexes;
};