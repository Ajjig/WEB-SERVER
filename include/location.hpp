#include "header.hpp"

class Location {
	public:
		Location(std::vector<str> config);
		~Location();
		void setPath(str path);
		str getPath();
		void setRoot(str root);
		str getRoot();
		void addIndex(str index);
		std::vector<str> getIndexes();
		void parse(std::vector<str> config);


	private:
		str					_root;
		std::vector<str>	_indexes;
};