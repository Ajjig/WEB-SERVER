#include "../../include/header.hpp"



int main(int argc, char *argv[], char **envp)
{
	std::string defined_dir = "/home/zoom/WEB-SERVER/html/cgi-bin/";
	std::string file = defined_dir + "hello.js";
	
	std::map<std::string, std::string> bin_path;
	bin_path.insert(std::pair<std::string, std::string>("php", "/usr/bin/php-cgi"));
	bin_path.insert(std::pair<std::string, std::string>("py", "/usr/bin/python3"));
	bin_path.insert(std::pair<std::string, std::string>("js", "/usr/bin/node"));
	bin_path.insert(std::pair<std::string, std::string>("cgi", getenv("PWD")));

	Cgi Cgi(file, bin_path, envp);
	std::cout << Cgi.get_response() << std::endl;

	
	return 0;
}
