#include <sys/socket.h>
#include <stdexcept>
#include <iostream>
#include <netinet/in.h>
#include "defines.h"
#include <stdio.h>
#include <unistd.h>

class HTTPServer
{
private:
    int in_sock;
    uint16_t port;

    void CreateListeningSocket()
    {
        this->in_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (this->in_sock == -1)
        {
            std::cout << SERR_PREFIX << "Socket creation failed. Errno = "
                      << errno << " Exiting..." << std::endl;
            exit(EXIT_CODE_SOCK_CREATE_ERROR);
        }

        int opt = 1;

        if (setsockopt(this->in_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        {
            std::cout << SERR_PREFIX << "Setting options for socket failed. Errno = "
                      << errno << "Exiting..." << std::endl;
            exit(EXIT_CODE_SOCK_SETOPT_ERROR);
        }

        sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(this->port);
        // bind to all addresses;
        sin.sin_addr.s_addr = INADDR_ANY;

        if (bind(this->in_sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
        {
            std::cout << SERR_PREFIX << "Bind socket failed. Errno = "
                      << errno << "Exiting..." << std::endl;
            exit(EXIT_CODE_SOCK_BIND_FAILED);
        }

        if (listen(this->in_sock, MAX_QUEUED_CONN_REQUESTS) != 0)
        {
            std::cout << SERR_PREFIX << "Failed to listen on socket. Errno = "
                      << errno << "Exiting..." << std::endl;
            exit(EXIT_CODE_SOCK_LISTEN_FAILED);
        }

        sockaddr_in s_addr_in;
        socklen_t size_addr;
        getsockname(this->in_sock, (struct sockaddr *)&s_addr_in, &size_addr);
        std::cout << SINFO_PREFIX << "Socket listening to port " << port
                  << " on all possible addresses. " << std::endl;
    }
    void WaitForConnections()
    {
        while (1)
        {
            int new_conn_sock;
            sockaddr_in sin;
            socklen_t len = sizeof(sin);
            new_conn_sock = accept(this->in_sock, (struct sockaddr *)&sin, &len);
            if (new_conn_sock == -1)
            {
                std::cout << SERR_PREFIX << "Error happened while accepting new connection. Errno = "
                          << errno << std::endl;
            }
            else
            {
                // be sure that we producing null-terminated cstring
                char buffer[MAX_CHUNK_SIZE + 1] = {0};
                int read_size = 0;
                std::cout << SINFO_PREFIX << "Accepting new connection." << std::endl;
                // while not EOF recieving chunks of request
                while (read_size = read(new_conn_sock, &buffer, MAX_CHUNK_SIZE) > 0)
                {
                    std::cout << SINFO_PREFIX << "Chunk recieved[" << read_size << "]" << std::endl;
                    std::cout << buffer;
                }
                if (read_size == -1)
                {
                    std::cout << SERR_PREFIX << "Error happened while recieving request chunk. Errno = "
                              << errno << std::endl;
                }
                std::cout << std::endl;
                if (shutdown(new_conn_sock, SHUT_RDWR) == -1)
                {
                    std::cout << "Error while shutdown incoming socket. Errno = "
                              << errno << std::endl;
                }
            }
        }
    }

public:
    void Serve()
    {
        CreateListeningSocket();
        WaitForConnections();
    }
    HTTPServer();
    HTTPServer(const uint16_t &port);
    ~HTTPServer();
};

HTTPServer::HTTPServer()
{
}

HTTPServer::~HTTPServer()
{
}
HTTPServer::HTTPServer(const uint16_t &port)
{
    this->port = port;
}