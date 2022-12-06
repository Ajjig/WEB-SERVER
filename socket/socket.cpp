#include "../include/socket.hpp"


/**
 *  @brief Construct a new Socket:: Socket object
 *  socket(domain, type, protocol)
 *  domain: AF_INET (IPv4 protocol) or AF_INET6 (IPv6 protocol)
 *  type: SOCK_STREAM (TCP) or SOCK_DGRAM (UDP)
 *  protocol: 0 (IP)
 */
Socket::Socket() : server_fd(0), new_socket(0), ready_descriptor(0), addrlen_cli(0), childpid(0)
{
    // Creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // returns 0 is error
    if (server_fd == 0)
    {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET; // The address family we used when we set up the socket.
    server_address.sin_addr.s_addr = htonl ( INADDR_ANY ); // machine’s IP address. htonl() converts the IP address to network byte order.
    server_address.sin_port = htons( PORT ); // port number to bind to. htons() converts the port number to network byte order.
}

Socket::~Socket() {}

/**
 * @brief Bind the socket to the address and port number specified in addr 
 * 
 * bind(server_fd, server_address, addrlen)
 *      - server_fd socket file descriptor
 *      - server_address server address
 *      - addrlen length of the address
 * 
 * The structure sockaddr is a generic container that 
 * just allows the OS to be able to read the first couple
 * of bytes that identify the address family. The address 
 * family determines what variant of the sockaddr struct to 
 * use that contains elements that make sense for that specific 
 * communication type. For IP networking, we use struct sockaddr_in, 
 * which is defined in the header netinet/in.h
 */
void Socket::bind()
{
    ::bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));
}

/**
 * @brief Listen for connections on a socket
 * listen(socket, backlog)
 * socket: socket file descriptor
 * backlog: maximum length to which the queue of pending connections for socket may grow
 * FD_ZERO: Initializes the file descriptor set fdset to have zero bits for all file descriptors.
 */
void Socket::listen()
{
    ::listen(server_fd, 10);
    FD_ZERO(&readfds);
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
    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
  
    return strStream.str();
}

std::string Socket::construct_response()
{
    std::string out = read_file((char *)"./socket/index.html");
    int file_size = out.length();

    std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " \
        + std::to_string(file_size) + "\n\n" + out;

    return response ;
}

void Socket::accept_socket()
{
    for (;;)
    {
        FD_SET(server_fd, &readfds);
        ready_descriptor = select(server_fd + 1, &readfds, NULL, NULL, NULL);
        
        if (FD_ISSET(server_fd, &readfds))
        {
            addrlen_cli = sizeof(client_address);
            new_socket = accept(server_fd, (struct sockaddr *)&client_address, &addrlen_cli);
            if (!(childpid = fork()))
            {
                close(server_fd);
                bzero(buffer, MAXLINES);
                read( new_socket , buffer, sizeof(buffer));
                std::cout << buffer << std::endl;
                std::string response = construct_response();
                write(new_socket, response.c_str(), response.length());
                close(new_socket);
                exit(0);
            }
            close(new_socket);
        }
    }
}

void Socket::start()
{
    bind();
    listen();
    accept_socket();
}