#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
# include <fstream>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <map>
#include <utility>

#include "../srcs/respond/res.hpp"
#include "../include/header.hpp"

#define MAX_EVENTS 10
#define MAX_FD 200

class Socket
{
    private:
        // using poll instead of epoll
        struct pollfd fds[MAX_FD];
        int nfds;
        socklen_t addrlen;
        int master_socket, incoming_connection, N_Files_discriptors, pfd, fd, i, nread, n;
        struct sockaddr_in local, remote;
        char http_header[BUFSIZ];
        int close_connection;
        int REQ_COUNT;
        int _port;
        std::string _host;
        std::vector<int> master_socket_list;
        std::vector<Server> __server_list;
        
        std::string construct_response();
        std::string read_file(char *filename);
        std::string get_port_from_fd(int _master_socket_fd);
        int current_interface_index(int _master_socket_fd);
        std::string parse_server_name(std::string server_name);
    
    public:
        Socket(int port, std::string host);
        Socket(std::vector<Server> servers);
        ~Socket();
        int set_nonblocking(int sockfd);
        int init_socket(int defined_port, std::string defined_host);
        void init_poll(int defined_master_socket);
        Server current_server(int _master_socket_fd, std::string server_name);
        void set_incoming_connection();
        void read_fd();
        void write_fd(std::string res);
        void log_client_info(int master_socket);
        void start();
        int is_master_socket(int fd);
        void setup_multiple_interface(std::vector<Server> interface_list);
        int get_port();
        std::string get_host();
        std::string get_http_header();
};