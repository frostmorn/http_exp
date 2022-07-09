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
#endif
