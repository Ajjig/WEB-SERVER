#include "../include/socket.hpp"

Socket::Socket() : nfds(1)
{
	master_socket = this->init_socket();
	this->set_nonblocking(master_socket);
	this->init_poll();
}

Socket::~Socket()
{
	// close(epfd);
	close(master_socket);
}

int Socket::init_socket()
{
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

void Socket::init_poll()
{
	memset(fds, 0, sizeof(fds));
	fds[0].fd = master_socket;
	fds[0].events = 0 | POLLIN;
	// nfds++;
}

int Socket::set_nonblocking(int sockfd)
{
	if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl(F_SETFL)\n");
        exit(1);
    }
	return 1;
}

void Socket::set_incoming_connection()
{
	while ((incoming_connection = accept(master_socket,(struct sockaddr *) &remote, &addrlen)) > 0)
	{
		set_nonblocking(incoming_connection);
		// using poll
		fds[nfds].fd = incoming_connection;
		fds[nfds].events = 0 | POLLIN;
		nfds++;
		std::cout << "New connection : " << incoming_connection <<std::endl;
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
	{
		perror("read error");
		close(fd);
	}
	else if (nread == 0)
	{
		perror("Client disconnected upexpectedly");
		// close(fd);
		fds[i].fd = -1;
		nfds--;
	}
	else
	{
		fds[i].events = POLLOUT;
		std::cout << "Read " << n << " bytes from socket " << fd << std::endl;
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
	fds[i].fd = -1;
}

void Socket::start()
{
	for (;;)
	{
		// using poll instead of epoll
		int ret = poll(fds, nfds, 0);
		if (ret  == -1)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}
		else if (ret > 0)
		{
			N_Files_discriptors = nfds;


			for (i = 0; i < N_Files_discriptors; ++i)
			{
				fd = fds[i].fd;
				if (fds[i].revents & POLLIN)
				{
					if (fd == master_socket)
					{
						set_incoming_connection();
						continue;
					}
					else
					{
						std::cout << "Reading from socket " << fd << std::endl;
						read_fd();
					}

				}
				else if (fds[i].revents & POLLOUT)
				{
					write_fd();
				}
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

    inFile.open(filename, std::ios::binary);
    if (not inFile.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        // exit(1);
    }
    /* A stringstream associates a string object with a stream allowing you
	to read from the string as if it were a stream (like cin). */
    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
	
	inFile.close();

    return strStream.str();
}

std::string Socket::construct_response()
{

	request req(get_http_header());
	req.req_logs();
	respond res(req);
	res.Get();

	//std::cout << res.get_response() << std::endl;

    //std::string out = read_file((char *)"./html/test.json");

    //int file_size = out.size();

    //std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json; Connection: keep-alive; Content-Transfer-Encoding: binary; Content-Length: " \
    //    + std::to_string(file_size) + ";\r\n\r\n" + out;


    // std::string response = std::string("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") \
    //     + std::string("5") + std::string("\n\n") + std::string("Hello");

    return res.get_response() ;
}