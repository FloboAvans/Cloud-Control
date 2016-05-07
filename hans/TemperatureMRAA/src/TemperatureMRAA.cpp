#include "ClientSocket.h"
#include "mraa.hpp"
#include "SocketException.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <signal.h>

bool end {false};

void signalHandler(int signalValue) {
    std::cout << "Caught signal: " << signalValue << '\n';
    end = true;
}

int main() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    const int B = 4275;
    float temperature;
    mraa::Aio *a0 = NULL;
    a0 = new mraa::Aio(0);
    if (a0 == NULL) {
        std::cerr << "Can't create mraa::Aio object, exiting" << std::endl;
        return mraa::ERROR_UNSPECIFIED;
    }

    ClientSocket clientSocket("localhost", 41234);
    try {
        clientSocket.connect();
    } catch (SocketException& ex) {
        std::cout << "error: " << ex.description();
        return 1;
    }


    a0->read(); //First value is bogus, so that is fixed now....
    std::cout << "Started!\n";
    while (!end) {
        // Take the average of 5000 samples to calculate the temperature. The sensor heats up due to th current through
        // the sensor and therefore the temperature it registers varies. By taking the average this 'error' is avoided.
        float temperatureSum = 0;
        for (int i=0; i < 5000; i++) {
            int sensorValue = a0->read();
            float sensorFloatValue = (float) sensorValue;
            float resistance =
                    (1023.0f - sensorFloatValue) * 100000.0f / sensorFloatValue; //get the resistance of the sensor;
            float calculatedTemperature = 1 / (log(resistance / 100000.0f) / B + 1.0f / 298.15f) - 274.35f;//convert to temperature
            temperatureSum +=calculatedTemperature;
        }
        temperature = temperatureSum/5000.0;

        std::string stringJSON = "{\"n\": \"temp\", \"v\": ";
        stringJSON.append(std::to_string(temperature));
        stringJSON.append("}\n");

        try {
            clientSocket << stringJSON;
            std::cout << "send: " << stringJSON;
        } catch (SocketException &ex) {
            std::cout << "error: " << ex.description();
        }
        sleep(1);
    }
    return mraa::SUCCESS;
}
