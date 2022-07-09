#include "HTTPRequest.h"
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

