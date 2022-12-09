#include "header.hpp"

class Location {
	public:
		Location(std::vector<std::string> config);
		~Location();
		void setPath(std::string path);
		std::string getPath();
		void setRoot(std::string root);
		std::string getRoot();
		void addIndex(std::string index);
		std::vector<std::string> getIndexes();
		void parse(std::vector<std::string> config);


	private:
		std::string					_root;
		std::vector<std::string>	_indexes;
};