#include "header.hpp"

class Server {
	public:
		Server();
		~Server();
		void addLocation(str location);
		void addIndex(str index);
	private:
		std::vector<Location> locations;
		str _root;
		str _name;
};