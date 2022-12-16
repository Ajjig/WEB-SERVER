
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
        Cgi(Server server, std::string file);
        ~Cgi ();

        std::string exec_cgi();
        void find_bin();
        int check_file(std::string file);
        int check_bin(std::string bin);
        void set_body(std::string buf);
        std::string get_body();
        /*
           On sucess get_body() returns the body of the cgi script
            On failure get_body() returns the status code of the cgi script
                - 404: Not Found
                - 403: Forbidden
                - bin not found: bin not found
         */
};
