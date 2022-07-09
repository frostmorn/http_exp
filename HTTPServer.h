#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include "HTTPConnection.h"
#include "HTTPServer.h"

#include <sys/socket.h>
#include <stdexcept>
#include <iostream>
#include <netinet/in.h>
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include "HTTPConnection.h"
class HTTPServer
{
private:
  int in_sock;
  uint16_t port;
  std::vector<HTTPConnection*> connections;

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
    socklen_t size_addr = sizeof(s_addr_in);
    memset(&s_addr_in, 0, size_addr);
    getsockname(this->in_sock, (struct sockaddr *)&s_addr_in, &size_addr);
    
    this->port = this->port == 0 ? ntohs(s_addr_in.sin_port):this->port;

    std::cout << SINFO_PREFIX << "Socket listening to port " << this->port
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
        this->connections.push_back(new HTTPConnection(new_conn_sock));
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
  for (auto connection:this->connections){
    delete connection;
  }
}
HTTPServer::HTTPServer(const uint16_t &port)
{
  this->port = port;
}
#endif
