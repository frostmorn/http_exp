#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H
#include "defines.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <mutex>
#include "HTTPRequest.h"
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
#endif
