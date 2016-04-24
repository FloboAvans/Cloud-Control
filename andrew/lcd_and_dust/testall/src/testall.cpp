#include <iostream>
#include <string>
#include <math.h>
#include <future>
#include <thread>
#include <chrono>
#include <utility>
#include <functional>
#include <unistd.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

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

	//getData can take half a minute so put on another thread
	auto dustAsyncFuture = async(std::launch::async,[&]{return dust->getData();});

	try{
		lcd = new upm::Jhd1313m1(0, 0x3E, 0x62);
	} catch(const std::exception& e) {
		cout << "didn't initialize lcd" << endl;
	}

	if (lcd!=nullptr){
		lcd->setCursor(0,0);
		lcd->write("Cloud Control");
	}else{
		cout << "no lcd" << endl;
	}

	//generic readers of pins on device, whatever is connected:
	auto a = Gpio(0, true, false);
	auto b = Gpio(4, true, false);
	auto aa = Aio(0);

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

		lcd->setCursor(1,2);
		//extra spaces to clear previous pixels:
		string t {"Temp:"+to_string(temperature)+"            "};
		lcd->write(string(t));

		//dust sensor thread control
		std::future_status status;
		//check if future is ready
		status = dustAsyncFuture.wait_for(std::chrono::seconds(0));
		if (status == std::future_status::ready){
			data = dustAsyncFuture.get();
			cout << "Low pulse occupancy: " << data.lowPulseOccupancy << endl;
			cout << "Ratio: " << data.ratio << endl;
			cout << "Concentration: " << data.concentration << endl;


		    int sockfd;
		    struct sockaddr_in servaddr;
		    std::string stringJSON = "{\"n\": \"dust\", \"v\": ";
		    stringJSON.append(std::to_string(data.concentration));
		    stringJSON.append("}\n");
		    char json[100];
		    strcpy(json, stringJSON.c_str());

		    sockfd=socket(AF_INET,SOCK_DGRAM,0);
		    bzero(&servaddr,sizeof(servaddr));
		    servaddr.sin_family = AF_INET;
		    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
		    servaddr.sin_port=htons(41234);

		    sendto(sockfd,json,strlen(json),0,(struct sockaddr *)&servaddr,sizeof(servaddr));

		    printf("Sent %s", json);


			dustAsyncFuture = async(std::launch::async,[&]{return dust->getData();});

			string t {"Dust:"+to_string((int)data.concentration)+" pcs/L            "};
			lcd->setCursor(0,0);
			lcd->write(t);
		} else if (status == std::future_status::deferred){
			cout << "dust data is deferred" << endl;
		} else if (status == std::future_status::timeout){
			cout << "dust data is timeout" << endl;
		}

		sleep(3); //goodnight
	}

	return 0;
}
