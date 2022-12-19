#include "../../include/header.hpp"

class Location {
	public:
		Location() {};
		Location(std::vector<std::string> & config, size_t & i);
		~Location();
		void setPatorh(std::string path);
		void setRoot(std::string root);
		std::string getRoot();
		void addIndex(std::string index);
		std::vector<std::string> getIndexes();
		void parse(std::vector<std::string> & config, size_t & i);
		void setAllowed(std::string allowed);
		std::vector<std::string> getAllowed();
		bool isAllowed(std::string method);
		bool isAutoindex();


	private:
		std::string					_root;
		std::vector<std::string>	_indexes;
		std::vector<std::string>	_allowed;
		bool						_autoindex;
};