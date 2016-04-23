
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mraa.hpp"
#define DEFAULT_IOPIN 8

static int iopin;
int running = 0;

void sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("closing IO%d nicely\n", iopin);
        running = -1;
    }
}
int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Provide an int arg if you want to flash on something 
other than %d\n", DEFAULT_IOPIN);
        iopin = DEFAULT_IOPIN;
    } else {
        iopin = strtol(argv[1], NULL, 10);
    }

    signal(SIGINT, sig_handler);

    //! [Interesting]
    mraa::Gpio* gpio = new mraa::Gpio(iopin);
    if (gpio == NULL) {
        return mraa::ERROR_UNSPECIFIED;
    }
    mraa::Result response = gpio->dir(mraa::DIR_OUT);
    if (response != mraa::SUCCESS) {
        mraa::printError(response);
        return 1;
    }

    while (running == 0) {
        response = gpio->write(1);
        sleep(1);
        response = gpio->write(0);
        sleep(1);
    }
    delete gpio;
    return response;
    //! [Interesting]
}

