//
// Created by Hans Klabbers on 06-05-16.
//
#ifndef TEMPERATUREMRAA_SOCKETEXCEPTION_H
#define TEMPERATUREMRAA_SOCKETEXCEPTION_H

#include <string>
class SocketException {
public:
    SocketException(std::string s) : exceptionString(s) { };
    ~SocketException() { };
    std::string description() { return exceptionString; }

private:
    std::string exceptionString;
};

#endif //TEMPERATUREMRAA_SOCKETEXCEPTION_H
