#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H
#include "defines.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include "HTTPRequest.h"
class HTTPConnection
{
private:
    int sock_fd;  
    std::vector<HTTPRequest *> requests;
public:
    HTTPConnection();
    HTTPConnection(int sock_fd){
        this->sock_fd = sock_fd;
        //Read();
    }
    void Read(){
        // be sure that we producing null-terminated cstring
        std::string ReadData;
        char buffer[MAX_CHUNK_SIZE + 1] = {0};
        int read_size = 0;
        std::cout << SINFO_PREFIX << "Accepting new connection." << std::endl;
        // while not EOF recieving chunks of request
        while (read_size = read(this->sock_fd, &buffer, MAX_CHUNK_SIZE) > 0)
        {
            std::cout << SINFO_PREFIX << "Chunk recieved[" << read_size << "]" << std::endl;
            std::cout << buffer;
            ReadData.append(buffer);
        }
        this->requests.push_back(new HTTPRequest(this, ReadData));
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
    void Write(){

    }
    void Close(){

    }
    ~HTTPConnection();
};

HTTPConnection::HTTPConnection()
{
}

HTTPConnection::~HTTPConnection()
{
    this->Close();
 
    for (auto request:this->requests){
        delete request;
    }
}
#endif