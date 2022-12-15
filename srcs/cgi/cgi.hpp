
#pragma once

# include "../../include/header.hpp"

class Cgi 
{
    private:
        char **envp;
        std::map<std::string, std::string> _bin_path;
        std::string _bin;
        std::string _file;

    public:
        Cgi(std::string file ,std::map<std::string, std::string> bin_path, char **envp);
        ~Cgi ();

        void exec_cgi();
};
