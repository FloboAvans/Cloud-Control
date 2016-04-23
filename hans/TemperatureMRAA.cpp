/*
 * Author: Jessica Gomez <jessica.gomez.hernandez@intel.com>
 * Copyright (c) 2015 - 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file
 * @ingroup basic
 * @brief On board LED blink C++
 *
 * Blink the on board LED by writing a digital value to an output pin
 * using the IO mraa library.
 *
 * @date 29/09/2015
 */

#include "mraa.hpp"
#include <iostream>
#include <unistd.h>
#include <math.h>

/*
 * On board LED blink C++ example
 *
 * Demonstrate how to blink the on board LED, writing a digital value to an
 * output pin using the MRAA library.
 * No external hardware is needed.
 *
 * - digital out: on board LED
 *
 * Additional linker flags: none
 */

int main()
{
	const int B=3975;
	mraa::Aio* a0 = NULL;
	a0 = new mraa::Aio(0);
	if (a0 == NULL) {
		std::cerr << "Can't create mraa::Aio object, exiting" << std::endl;
		return mraa::ERROR_UNSPECIFIED;
	}

	std::cout << "Started!\n";
	for (;;) {
		int sensorValue = a0->read();
		//std::cout << sensorValue << std::endl;
		float resistance=(float)(1023-sensorValue)*10000/sensorValue; //get the resistance of the sensor;
		float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature
		std::cout << "Temperature: " << temperature << std::endl;
		sleep(1);
	}

	return mraa::SUCCESS;
}
