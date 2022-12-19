#include "../include/header.hpp"

Socket::Socket(int port, std::string host) : nfds(1), REQ_COUNT(0) ,_port(port), _host(host)
{
	memset(fds, 0, sizeof(fds));
}



Socket::Socket(std::vector<Server> servers) : nfds(1), REQ_COUNT(0), __server_list(servers)
{
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
	(void)master_socket;
	//std::cout << "New request on " << "Port : \033[32m" << get_port_from_fd(master_socket) << "\033[0m"<<std::endl;
}

int Socket::current_interface_index(int _master_socket_fd)
{
	for (int i = 0; i < (int )master_socket_list.size(); ++i)
	{
		if (_master_socket_fd == master_socket_list[i])
			return i;
	}
	return -1;
}

bool is_in_vector(std::vector<std::string> vec, std::string val)
{
	for (int i = 0; i < static_cast<int>(vec.size()); ++i)
	{
		// remove whitespace 
		val.erase(std::remove(val.begin(), val.end(), ' '), val.end());
		if (vec[i] == val)
			return true;
	}
	return false;
}

Server Socket::current_server(int _master_socket_fd, std::string server_name)
{
	for (int i = 0; i < static_cast<int>(__server_list.size()); ++i)
	{
		
		
		if (this->get_port_from_fd(_master_socket_fd) == std::to_string(__server_list[i].getPort()) && server_name == "Not defined")
		{
			return __server_list[i];
		}
		else if (this->get_port_from_fd(_master_socket_fd) == std::to_string(__server_list[i].getPort()) \
			&& is_in_vector(__server_list[i].getNames(), server_name))
		{
			return __server_list[i];
		}
	}

	/*
		the default server should handle the request if 
		all the requests that don’t belong to an other server.
	*/ 
	return __server_list[0];
}



std::string Socket::get_port_from_fd(int _master_socket_fd)
{
	int index = current_interface_index(_master_socket_fd);
	std::vector<Server>::iterator it = __server_list.begin();
	std::advance(it, index);
	return std::to_string(it->getPort());
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
		//perror("Client disconnected upexpectedly");
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

	const char *buffer = res.c_str();

	int nwrite, data_size = len;
	n = data_size;
	while (n > 0) {
		nwrite = write(fd, buffer + data_size - n, n);
		if (nwrite < n) {
			if (nwrite == -1 && errno != EAGAIN) {
				perror("write error");
			}
		}
		n -= nwrite;
	}
	close(fd);
	fds[i].fd = -1;
}

int Socket::is_master_socket(int __fd)
{
	for (int i = 0; i < (int )master_socket_list.size(); ++i)
	{
		if (__fd == master_socket_list[i])
		{
			master_socket = __fd;
			return 1;
		}
	}
	return 0;
}

void Socket::setup_multiple_interface(std::vector<Server> interface_list)
{
	(void )interface_list;
	for (std::vector<Server>::iterator it = __server_list.begin(); it != __server_list.end(); ++it)
	{
		int socket_id;

		if (it->isBind() == true)
		{
			std::string fix_host = it->getHost();
			if (fix_host == "localhost")
				fix_host = "0.0.0.0";
			socket_id = this->init_socket(it->getPort(), fix_host);
			this->set_nonblocking(socket_id);
			this->init_poll(socket_id);
		}

		std::cout << " * Running on http://" << it->getHost() <<  ":\033[34m" << it->getPort() << "\033[0m/ "\
			<< "\tServer Name:\033[32m " << (it->getName() != "" ? it->getName() : "Not Defined") <<  \
			 " \033[31m(Press CTRL+C to quit)\033[0m" <<  std::endl;
	}

	std::cout  << " * Restarting with stat" << std::endl;
	std::cout << " * Debugger is\033[32m active!\033[0m\n" << std::endl;
}


void Socket::start()
{
	setup_multiple_interface(this->__server_list);

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
std::string Socket::parse_server_name(std::string header)
{
	std::string tmp = header.find("server_name: ") != std::string::npos ? header.substr(header.find("server_name: ") + 12) : "";
	std::string ret = tmp.substr(0, tmp.find("\r\n"));
	if (ret == "")
		ret = "Not defined";

	return ret;
}

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
	// std::string name = current_server(master_socket, parse_server_name(get_http_header())).getName();

	
	Request req(get_http_header(), current_server(master_socket, parse_server_name(get_http_header())));
	// acitve logs
	//req.req_logs();

	std::cout << get_http_header() << std::endl;

	Respond res(req);

    return res.get_response();
	// return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n\r\n<html><body><h1>hello world</h1></body></html>";
}