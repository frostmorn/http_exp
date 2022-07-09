#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include "defines.h"
#include <string>
#include <iostream>
class HTTPConnection;
class HTTPRequest
{
private:
  /* data */
  HTTPConnection *conn;
  std::string req_data;
  void ParseHeaders();
public:
  HTTPRequest(HTTPConnection *conn, std::string &req_data);
  ~HTTPRequest();
};

HTTPRequest::HTTPRequest(HTTPConnection *conn, std::string &req_data)
{
  std::cout << SINFO_PREFIX << "HTTPRequest created" << std::endl;
  this->conn = conn;   
  this->req_data = req_data; 
}

void HTTPRequest::ParseHeaders(){


}

HTTPRequest::~HTTPRequest()
{
}
#endif
