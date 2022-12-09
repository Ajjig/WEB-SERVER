#pragma once

#include <iostream>
#include <sstream>
#include <string>

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
        char buffer[BUFSIZ];
        char http_header[BUFSIZ];
        int close_connection;
        int REQ_COUNT;
        std::string construct_response();
        std::string read_file(char *filename);
        int _port;
        std::string _host;

    public:
        Socket();
        Socket(int port, std::string host);
        Socket(int port);
        ~Socket();
        int set_nonblocking(int sockfd);
        int init_socket();
        void init_poll();

        void set_incoming_connection();
        void read_fd();
        void write_fd();
        void log_client_info();
        void start();

        int get_port();
        std::string get_host();
        std::string get_http_header();
};