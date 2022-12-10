#include "../include/socket.hpp"

Socket::Socket(std::map<std::string, std::string> interface_list) : nfds(1), REQ_COUNT(0)
{
	__interface_list = interface_list;
	memset(fds, 0, sizeof(fds));
}

Socket::~Socket()
{
	close(master_socket);
}

int Socket::init_socket(int defined_port, std::string defined_host)
{
	_port = defined_port;
	_host = defined_host;
	if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("sockfd ");
		exit(1);
	}

	if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("sockfd ");
        exit(1);
    }

	bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr((_host.c_str()));
    local.sin_port = htons(_port);
	
	int optval = 1;
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt ");
		exit(1);
	}
	
	if( bind(master_socket, (struct sockaddr *) &local, sizeof(local)) < 0) {
        perror("bind ");
        exit(1);
    }

    listen(master_socket, MAX_FD);
	return master_socket;
}

void Socket::init_poll(int defined_master_socket)
{
	static int interface_index = 0;

	
	fds[interface_index].fd = defined_master_socket;
	fds[interface_index].events = 0 | POLLIN;
	interface_index++;
	nfds = interface_index;
	master_socket_list.push_back(defined_master_socket);
}

int Socket::set_nonblocking(int sockfd)
{
	if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl(F_SETFL)\n");
        exit(1);
    }
	return 1;
}

void Socket::log_client_info(int master_socket)
{
	REQ_COUNT++;
	std::cout << "\tNew client on " << "Port : \033[32m" << get_port_from_fd(master_socket) << "\033[0m"<<std::endl;
}

int Socket::current_interface_index(int _master_socket_fd)
{
	for (int i = 0; i < master_socket_list.size(); ++i)
	{
		if (_master_socket_fd == master_socket_list[i])
			return i;
	}
	return -1;
}

std::string Socket::get_port_from_fd(int _master_socket_fd)
{
	int index = current_interface_index(_master_socket_fd);
	std::map<std::string, std::string>::iterator it = __interface_list.begin();
	std::advance(it, index);
	return it->first;
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
		log_client_info(master_socket);
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
		fds[i].fd = -1;
		nfds--;
	}
	else
	{
		fds[i].events = POLLOUT;
	}

}

void Socket::write_fd(std::string res)
{
	int len = res.length();
	
	char buffer[len] = {0};
	strcpy(buffer, res.c_str());

	int nwrite, data_size = len;
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

int Socket::is_master_socket(int __fd)
{
	for (int i = 0; i < master_socket_list.size(); ++i)
	{
		if (fd == master_socket_list[i])
		{
			master_socket = __fd;
			return 1;
		}
	}
	return 0;
}

void Socket::setup_multiple_interface(std::map<std::string, std::string> interface_list)
{
	for (std::map<std::string, std::string>::iterator it = interface_list.begin(); it != interface_list.end(); ++it)
	{
		int test;
		test = this->init_socket(atoi(it->first.c_str()), it->second);	
		this->set_nonblocking(test);
		this->init_poll(test);
	}

	std::cout << " * Running on http://" << this->_host <<  ":" << this->_port << " \033[31m(Press CTRL+C to quit)\033[0m" <<  std::endl;
	std::cout  << " * Restarting with stat" << std::endl;
	std::cout << " * Debugger is\033[32m active!\033[0m\n" << std::endl;
}


void Socket::start()
{
	setup_multiple_interface(__interface_list);

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
					if (is_master_socket(fd))
					{
						set_incoming_connection();
						continue;
					}
					else
					{
						read_fd();
					}

				}
				else if (fds[i].revents & POLLOUT)
				{
					write_fd(construct_response());
				}
			}
		}
	}
}

int Socket::get_port()
{
	return _port; // to be changed later
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
	// acitve logs
	req.req_logs();

	respond res(req);
	if (req.get_method() == "GET")
		res.Get();

	// std::cout << res.get_response() << std::endl;

    // std::string out = read_file((char *)"./html/index.html");

    // int file_size = out.size();

    // std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json; Connection: keep-alive; Content-Transfer-Encoding: binary; Content-Length: " \
    //    + std::to_string(file_size) + ";\r\n\r\n" + out;

    // std::string response = std::string("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ") \
    //     + std::string("5") + std::string("\n\n") + std::string("Hello");

    return res.get_response();
}