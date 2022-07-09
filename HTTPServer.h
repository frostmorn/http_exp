#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "HTTPConnection.h"
#include <sys/socket.h>
#include <stdexcept>
#include <iostream>
#include <netinet/in.h>
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <vector>
class HTTPServer
{
private:
  int in_sock;
  uint16_t port;
  std::vector<HTTPConnection*> connections;
  void CreateListeningSocket();
  void  WaitForConnections();
public:
  HTTPServer();
  HTTPServer(const uint16_t &port);
  void Serve();
  ~HTTPServer();
};
#endif
