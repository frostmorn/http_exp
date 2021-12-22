#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H
#include "defines.h"
#include <iostream>
class HTTPConnection
{
private:
    int sock_fd;    
public:
    HTTPConnection();
    HTTPConnection(int sock_fd){
        this->sock_fd = sock_fd;
        // be sure that we producing null-terminated cstring
        char buffer[MAX_CHUNK_SIZE + 1] = {0};
        int read_size = 0;
        std::cout << SINFO_PREFIX << "Accepting new connection." << std::endl;
        // while not EOF recieving chunks of request
        while (read_size = read(this->sock_fd, &buffer, MAX_CHUNK_SIZE) > 0)
        {
            std::cout << SINFO_PREFIX << "Chunk recieved[" << read_size << "]" << std::endl;
            std::cout << buffer;
        }
        if (read_size == -1)
        {
            std::cout << SERR_PREFIX << "Error happened while recieving request chunk. Errno = "
                        << errno << std::endl;
            this->~HTTPConnection();
        }
        std::cout << std::endl;
        if (shutdown(this->sock_fd, SHUT_RDWR) == -1)
        {
            std::cout << "Error while shutdown incoming socket. Errno = "
                        << errno << std::endl;
            this->~HTTPConnection();
        }
    }
    ~HTTPConnection();
};

HTTPConnection::HTTPConnection()
{
}

HTTPConnection::~HTTPConnection()
{
}
#endif