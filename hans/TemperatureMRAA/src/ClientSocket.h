//
// Created by Hans Klabbers on 06-05-16.
//
#ifndef TEMPERATUREMRAA_CLIENTSOCKET_H
#define TEMPERATUREMRAA_CLIENTSOCKET_H

#include "Socket.h"

class ClientSocket : private Socket {
public:

    ClientSocket(std::string host, int port);
    virtual ~ClientSocket() { };
    void connect();
    const ClientSocket &operator<<(const std::string &) const;
    const ClientSocket &operator>>(std::string &) const;

private:
    std::string host;
    int port;
};

#endif //TEMPERATUREMRAA_CLIENTSOCKET_H
