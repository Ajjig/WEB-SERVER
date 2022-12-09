#include "../include/socket.hpp"

Socket::Socket()
{
	master_socket = this->init_socket();
	this->set_nonblocking(master_socket);
	this->init_epoll();
}

Socket::~Socket()
{
	close(epfd);
	close(master_socket);
}

int Socket::init_socket()
{
	if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {    
        perror("sockfd\n");    
        exit(1);    
    }

	if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {    
        perror("sockfd\n");    
        exit(1);    
    }    
	bzero(&local, sizeof(local)); 
    local.sin_family = AF_INET;    
    local.sin_addr.s_addr = htonl(INADDR_ANY);;    
    local.sin_port = htons(this->get_port());    

	int optval = 1;
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	if( bind(master_socket, (struct sockaddr *) &local, sizeof(local)) < 0) {    
        perror("bind\n");    
        exit(1);    
    }    
    listen(master_socket, MAX_FD);

	return master_socket;
}

void Socket::init_epoll()
{
	epfd = epoll_create(MAX_EVENTS);    
    if (epfd == -1) {    
        perror("epoll_create");    
        exit(EXIT_FAILURE);    
    }      
    ev.events = EPOLLIN;    
    ev.data.fd = master_socket;    
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, master_socket, &ev) == -1) {    
        perror("epoll_ctl: listen_sock");    
        exit(EXIT_FAILURE);    
    }
}

int Socket::set_nonblocking(int sockfd)
{
	if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {    
        perror("fcntl(F_SETFL)\n");    
        exit(1);    
    }
}

void Socket::set_incoming_connection()
{
	while ((incoming_connection = accept(master_socket,(struct sockaddr *) &remote, &addrlen)) > 0) 
	{    
		set_nonblocking(incoming_connection); 
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = incoming_connection;    
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, incoming_connection,&ev) == -1) 
		{    
			perror("epoll_ctl: add");    
			exit(EXIT_FAILURE);    
		}    
	}    
	if (incoming_connection == -1) 
	{    
		if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)     
			perror("accept");    
	}    
}

void Socket::read_fd()
{
	n = 0;    
	while ((nread = read(fd, http_header + n, BUFSIZ-1)) > 0) 
	{    
		n += nread;    
	}    
	if (nread == -1 && errno != EAGAIN) 
		perror("read error");

	ev.data.fd = fd;    
	ev.events = events[i].events | EPOLLOUT;    

	if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1) 
	{    
		perror("epoll_ctl: mod");    
	}  
}

void Socket::write_fd()
{
	std::string response = construct_response();
	
	sprintf(buffer, response.c_str(), response.length());    

	int nwrite, data_size = strlen(buffer);    
	n = data_size;    
	while (n > 0) {    
		nwrite = write(fd, buffer + data_size - n, n);    
		if (nwrite < n) {    
			if (nwrite == -1 && errno != EAGAIN) {    
				perror("write error");    
			}    
			break;    
		}    
		n -= nwrite;    
	}    
	close(fd);    
}

void Socket::start()
{
	for (;;)
	{
		N_Files_discriptors = epoll_wait(epfd, events, MAX_EVENTS, -1); 
		if (N_Files_discriptors == -1) 
		{    
			perror("epoll_pwait");    
			exit(EXIT_FAILURE);    
		}
		for (i = 0; i < N_Files_discriptors; ++i)
		{
			fd = events[i].data.fd;    
            if (fd == master_socket) 
			{
				set_incoming_connection();
				continue;
			}
			if (events[i].events & EPOLLIN) 
			{
				read_fd();
			}
			if (events[i].events & EPOLLOUT)
			{
				write_fd();
			}
		}
	}
}

int Socket::get_port()
{
	return PORT; // to be changed later
}

// Private methods
std::string Socket::get_http_header()
{
	return std::string(http_header);
}

std::string Socket::read_file(char *filename)
{
    std::ifstream inFile;

    inFile.open(filename, std::ios::in);
    if (not inFile.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }
    /* A stringstream associates a string object with a stream allowing you 
	to read from the string as if it were a stream (like cin). */
    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
  
    return strStream.str();
}

std::string Socket::construct_response()
{
    std::string out = read_file((char *)"../html/index.html");
    int file_size = out.length();

    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " \
        + std::to_string(file_size) + "\n\n" + out;

    return response ;
}