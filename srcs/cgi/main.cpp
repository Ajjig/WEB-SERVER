#include "../../include/header.hpp"



int main(int argc, char *argv[], char **envp)
{
	std::map<std::string, std::string> bin_path;
	std::string file = "simple.py";
	bin_path.insert(std::pair<std::string, std::string>("php", "/usr/bin/php-cgi"));
	bin_path.insert(std::pair<std::string, std::string>("py", "/usr/bin/python3"));
	bin_path.insert(std::pair<std::string, std::string>("js", "/usr/bin/node"));

	Cgi Cgi(file, bin_path, envp);
	Cgi.exec_cgi();
	// char *args[] = {NULL};

	// args[0] = "/usr/bin/python3"
    // args[1] = "simple.py";
    // args[2] = NULL;

	// execve("/usr/bin/python3", argv, envp);
	return 0;
}
