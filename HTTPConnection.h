#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H
#include "defines.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include "HTTPRequest.h"
#include <mutex>
class HTTPConnection
{
private:
  int sock_fd;
  std::mutex sock_mutex;  
  std::vector<HTTPRequest *> requests;
public:
  HTTPConnection();
  HTTPConnection(int &sock_fd);
  void Read();
  void Write(uint8_t* buffer, size_t &size);
  void Close();
  ~HTTPConnection();
};

HTTPConnection::HTTPConnection()
{
  std::cout << SINFO_PREFIX << "HTTPConection created by empty constuctor" << std::endl;
}

HTTPConnection::HTTPConnection(int &sock_fd)
{
  this->sock_fd = sock_fd;
  std::cout << SINFO_PREFIX << "HTTPConection created and initalized by socket descriptor" << std::endl;
  Read();
}

void HTTPConnection::Read(){
  this->sock_mutex.lock();
  std::string ReadData;
  uint8_t buffer[MAX_CHUNK_SIZE + 1]={0};
  int read_size = 0;

  std::cout << SINFO_PREFIX << "Accepting new connection." << std::endl;

  // while not EOF recieving chunks of request
  while ((read_size = read(this->sock_fd, &buffer, MAX_CHUNK_SIZE)) > 0)
  {
    std::cout << SINFO_PREFIX << "Chunk recieved[ length:" << read_size << " ]" << std::endl;
    std::cout << buffer;
    
    // be sure that we producing null-terminated cstring
    buffer[read_size] = 0;
    
    ReadData.append((char *)buffer);
    
   // is req ended?
    if ( buffer[read_size-1] == '\n'){
      this->requests.push_back(new HTTPRequest(this, ReadData));
    }
  }

  if (read_size == -1)
  {
    std::cout << SERR_PREFIX << "Error happened while recieving request chunk. Errno = "
      << errno << std::endl;
    this->~HTTPConnection();
  }

  std::cout << std::endl;

  this->sock_mutex.unlock();
}

void HTTPConnection::Write(uint8_t* buffer, size_t &size){
  this->sock_mutex.lock();

  this->sock_mutex.unlock();
}

void HTTPConnection::Close(){
  this->sock_mutex.lock();
  if (shutdown(this->sock_fd, SHUT_RDWR) == -1)
  {
    std::cout << "Error while shutdown incoming socket. Errno = "
      << errno << std::endl;
    this->~HTTPConnection();
  }
  this->sock_mutex.unlock();
}

HTTPConnection::~HTTPConnection()
{
  this->Close();
  for (auto request:this->requests){
    delete request;
  }
}
#endif
