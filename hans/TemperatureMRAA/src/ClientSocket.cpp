//
// Created by Hans Klabbers on 06-05-16.
//
#include <iostream>
#include "ClientSocket.h"
#include "SocketException.h"

ClientSocket::ClientSocket(std::string host, int port) : host(host), port(port) { }

void ClientSocket::connect() {
    if (!Socket::create()) {
        std::cout << "Socket::create() returned false.";
        throw SocketException("Could not create client socket.");
    }

    if (!Socket::connect(host, port)) {
        std::cout << "Socket::connect() returned false.";
        throw SocketException("Could not bind to port.");
    }
}

const ClientSocket &ClientSocket::operator<<(const std::string &message) const {
    if (!Socket::send(message)) {
        throw SocketException("Could not write to socket.");
    }
    return *this;
}

const ClientSocket &ClientSocket::operator>>(std::string &message) const {
    if (!Socket::recv(message)) {
        throw SocketException("Could not read from socket.");
    }
    return *this;
}