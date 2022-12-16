
#pragma once

# include "../../include/header.hpp"

class Cgi 
{
    private:
        char **envp;
        std::map<std::string, std::string> _bin_path;
        std::string _bin;
        std::string _file;
        std::string _buffer;

    public:
        Cgi(std::string file ,std::map<std::string, std::string> bin_path, char **envp);
        ~Cgi ();

        std::string exec_cgi();
        void find_bin();
        std::string start(std::string buf);
        std::string get_response();
};
