#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mraa/aio.h"

/*#define ANALOG_PIN 3*/

int main(int argc, char** argv)
{
    mraa_aio_context adc;
    uint16_t value;
    int pin = atoi(argv[1]);

    /* Init Pin */
    adc = mraa_aio_init(pin);
    if (adc == NULL) {
        fprintf(stderr, "[-] Cannot init pin %d\n", pin);
        return (1);
    }
    
    while (1) {
        /* Read the value on the Pin */
	sleep(3);
        value = mraa_aio_read(adc);
        fprintf(stdout, "Pin A%d received %hu\n", pin, value);
    }

    mraa_aio_close(adc);

    return 0;
}
