#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include <string>
class HTTPConnection;
class HTTPRequest
{
private:
    /* data */
    HTTPConnection *conn;
    std::string req_data;
public:
    HTTPRequest(HTTPConnection *conn, std::string &req_data);
    ~HTTPRequest();
};

HTTPRequest::HTTPRequest(HTTPConnection *conn, std::string &req_data)
{
    this->conn = conn;   
    this->req_data = req_data; 
}

HTTPRequest::~HTTPRequest()
{
}
#endif