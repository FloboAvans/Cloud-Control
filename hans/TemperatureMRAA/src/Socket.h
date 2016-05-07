//
// Created by Hans Klabbers on 06-05-16.
//
#ifndef TEMPERATUREMRAA_SOCKET_H
#define TEMPERATUREMRAA_SOCKET_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket {
public:
    Socket();

    virtual ~Socket();

    bool connect(const std::string host, const int port);
    bool send(const std::string message) const;
    bool create();
    bool is_valid() const { return serverPort != -1; }
    int recv ( std::string& message) const;

private:

    int serverPort;
    sockaddr_in serverAddress;

};

#endif //TEMPERATUREMRAA_SOCKET_H
