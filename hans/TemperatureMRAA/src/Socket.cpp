//
// Created by Hans Klabbers on 06-05-16.
//
#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>

Socket::Socket() : serverPort(-1) {
    memset(&serverAddress, 0, sizeof(serverAddress));
}

Socket::~Socket() {
    if (is_valid())
        ::close(serverPort);
}

bool Socket::send(const std::string s) const {
    int status = ::send(serverPort, s.c_str(), s.size(), MSG_NOSIGNAL);
    if (status == -1) {
        return false;
    } else {
        return true;
    }
}

int Socket::recv(std::string &message) const {
    char buf[MAXRECV + 1];

    message = "";

    memset(buf, 0, MAXRECV + 1);

    int status = ::recv(serverPort, buf, MAXRECV, 0);

    if (status == -1) {
        std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
        return 0;
    } else if (status == 0) {
        return 0;
    } else {
        message = buf;
        return status;
    }
}

bool Socket::connect(const std::string host, const int port) {
    if (!is_valid()) return false;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr);

    if (errno == EAFNOSUPPORT) return false;

    status = ::connect(serverPort, (sockaddr *) &serverAddress, sizeof(serverAddress));

    if (status == 0)
        return true;
    else
        return false;
}

bool Socket::create() {
    serverPort = socket(AF_INET, SOCK_DGRAM, 0);
    if (!is_valid())
        return false;

    // TIME_WAIT - argh
    int on = 1;
    if (setsockopt(serverPort, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on)) == -1)
        return false;

    return true;
}