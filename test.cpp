#include "HTTPServer.h"

int main(int argc, char** argv){
    auto server = HTTPServer(5000);
    server.Serve();
    return 0;
}