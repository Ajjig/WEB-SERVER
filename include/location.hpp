#include "header.hpp"

class Location {
	public:
		Location(std::vector<string> config);
		~Location();
		void setPath(string path);
		string getPath();
		void setRoot(string root);
		string getRoot();
		void addIndex(string index);
		std::vector<string> getIndexes();
		void parse(std::vector<string> config);


	private:
		string					_root;
		std::vector<string>	_indexes;
};