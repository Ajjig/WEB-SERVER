#pragma once

#include <iostream>
#include <sstream>
#include <string>

# include <fstream>
#include <sys/socket.h>    
#include <sys/wait.h>    
#include <netinet/in.h>    
#include <netinet/tcp.h>    

// error
#include <sys/epoll.h>    
#include <sys/sendfile.h>
// end

#include <sys/stat.h>    
#include <unistd.h>    
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>    
#include <strings.h>    
#include <fcntl.h>    
#include <errno.h>     

#define MAX_EVENTS 10    
#define PORT 80   
#define MAX_FD 20

class Socket
{
    private:
        struct epoll_event ev, events[MAX_EVENTS];    
        socklen_t addrlen;
        int master_socket, incoming_connection, N_Files_discriptors, epfd, fd, i, nread, n;    
        struct sockaddr_in local, remote;    
        char buffer[BUFSIZ];
        char http_header[BUFSIZ];
        
        std::string construct_response();
        std::string read_file(char *filename);
    
    public:
        Socket();
        ~Socket();
        int set_nonblocking(int sockfd);
        int init_socket();
        void init_epoll();

        void set_incoming_connection();
        void read_fd();
        void write_fd();

        void start();

        int get_port();
        std::string get_host();
        std::string get_http_header();
};