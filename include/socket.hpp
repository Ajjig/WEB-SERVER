#pragma once

#include "../include/header.hpp"

#define PORT 80
#define MAXLINES 1024

class Socket
{
    private:
        int server_fd; 
        int new_socket;
        int ready_descriptor;

        struct sockaddr_in server_address, client_address;
        fd_set readfds;
        socklen_t addrlen_cli;
        pid_t childpid;
        char buffer[MAXLINES];
        
        std::string construct_response();
        std::string read_file(char *filename);
    
    public:
        Socket();
        ~Socket();
        void bind();
        void listen();
        void accept_socket();
        void start();
};