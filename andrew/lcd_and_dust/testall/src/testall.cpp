#include <iostream>
#include <string>
#include <math.h>
#include "grove.h"
#include "mraa.h"

//libraries for dust sensor and lcd readout
#include "jhd1313m1.h"
#include "ppd42ns.h"

using namespace std;
using namespace mraa;

int main() {

	//const for temperature conversion
	const int B=3975;

	//dust sensor
	upm::PPD42NS* dust = new upm::PPD42NS(8);
	upm::dustData data;

	//lcd readout
	upm::Jhd1313m1 *lcd=nullptr;

	try{
		lcd = new upm::Jhd1313m1(0, 0x3E, 0x62);
	} catch(const std::exception& e) {
		cout << "didn't initialize lcd" << endl;
	}

	if (lcd!=nullptr){
		lcd->setCursor(0,0);
		lcd->write("Hello World");
		lcd->setCursor(1,2);
		lcd->write("Hello World");
	}else{
		cout << "no lcd" << endl;
	}

	//generic readers of pins on device, whatever is connected:
	auto a = Gpio(0, true, false);
	auto b = Gpio(4, true, false);
	auto aa = Aio(0);

//	auto dustSensor=Gpio(8, true, false);

	/* Code in this loop will run repeatedly
	 */
	for (;;) {
		//generic readers of pins on device, whatever is connected:
		cout << "gpio 0 is " << a.read() << endl;
		cout << "gpio 4 is " << b.read() << endl;
		cout << "aio 0 is " << aa.read() << endl;
		cout << "aio 0 float is " << aa.readFloat() << endl;

		int tempSensor = aa.read(); // my setup has temp sensor in this pin
		float resistance=(float)(1023-tempSensor)*10000/tempSensor; //get the resistance of the sensor;
		float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature

		lcd->setCursor(0,0);
		lcd->write("Cloud Control");
		lcd->setCursor(1,2);
		//extra spaces to clear previous pixels:
		string t {"Temp:"+to_string(temperature)+"            "};
		lcd->write(string(t));

		//getData() can take 30 seconds to run
		data = dust->getData();
		cout << "Low pulse occupancy: " << data.lowPulseOccupancy << endl;
		cout << "Ratio: " << data.ratio << endl;
		cout << "Concentration: " << data.concentration << endl;

		sleep(2); //goodnight
	}

	return 0;
}
