
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
        Server _server;
        std::map<std::string, std::string> _env;
        int _status_code;

        int check_file(std::string file);
        int check_bin(std::string bin);
        void find_bin();
        void set_env();
        std::string exec_cgi();
    
    public:
        Cgi(std::string file ,std::map<std::string, std::string> bin_path, char **envp);
        Cgi(Server server, std::string file);
        ~Cgi ();

        void set_body(std::string buf);
        std::string get_body();
        int get_status_code();
        void set_status_code(int status_code);
        /*
           On sucess get_body() returns the body of the cgi script
            On failure get_body() returns the status code of the cgi script
                - 404: Not Found
                - 403: Forbidden
                - bin not found: bin not found
         */
};
