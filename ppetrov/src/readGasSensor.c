
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mraa/aio.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#include <math.h>

//gas sensors are supposed to work at 5V only!
#define VOLTS 5.0

//polling time period in secs
#define TIMEOUT 1.0

//enable cloud integration or not
//#define CLOUD




//sensor gas type pivot value for RS/R0  = 1
static float sensor_gas_pivot_1[][1] =
{
	// offset compensation to 1000pm to mathch RS/R0=1 
	// value below is : 1 - offset (since smaller RS/R0 is more ppm)

	//LPG
	{0.22},//MQ2
	{1.0},//MQ5
	{0.0}//MQ9
};

//value is RS/R0
float CalcPPM(float value,int sensor, int gas)
{
	float ppm = 10.0;

	if( value < 1.0 )//HI
	{
		
		float _exp = (1.0 - value) - sensor_gas_pivot_1[sensor][gas]  + 3.0;
		ppm = pow(10,_exp);
	}
	else if( value < 3.0 )//LOW
	{
		//int idx = (int)
		ppm = 500.0;
	}

	return ppm;
}


float CalibrateR0(mraa_aio_context* aio_pin,int sensorType, int gasType)
{
	//this is LPG RS for pure air environment
        float SENSOR_RSR0_LPG[3] =
	{
	9.8,//MQ2
	6.5,//MQ5
	9.9//MQ9
	};
        

    	float sensor_volt = 0;
	float sensorValue = 0;
    	float RS_air = 0; // Get the value of RS via in a clear air
    	float R0 = 0; // Get the value of R0 via in LPG

	
	for(int x = 0 ; x < 100 ; x++)
	{
        	sensorValue = sensorValue + mraa_aio_read_float(*aio_pin)*VOLTS;
	}
	sensorValue = sensorValue/100.0;

	//printf("ahh: %f",sensorValue);

	sensor_volt = sensorValue;
	RS_air = (VOLTS-sensor_volt)/sensor_volt; // omit *RL
	R0 = RS_air/SENSOR_RSR0_LPG[sensorType]; // The ratio of RS/R0 is 9.9 in LPG gas

	return R0;
}



int main(int argc, char** argv)
{
    mraa_aio_context adc;
    //uint16_t value;
    int pin = 0;
    int sensor = 0;
    int gas = 0;

    if (argc < 3) 
    {
printf("Usage:\narg1:SensorType[0=MQ2][1=MQ5][2=MQ9]\narg2:AnalogPinNumber[0-3]\narg3:GasType[0=LPG]\n");
        return -1;
    } 
    else 
    {
sensor = strtol(argv[1], NULL, 10);        
pin = strtol(argv[2], NULL, 10);
gas = strtol(argv[3], NULL, 10);
        
if(sensor < 0 || sensor > 2) return -2;
if(pin < 0 || pin > 3) return -2;
if(gas < 0) return -2;
    } 


#ifdef CLOUD
    int sockfd;
    struct sockaddr_in servaddr;
    char json[100];
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(41234);
#endif    

    float sensor_volt;
    //float RS_air; // Get the value of RS via in a clear air
    float R0; // Get the value of R0 via in LPG
    float sensorValue;

    // Init Pin
    adc = mraa_aio_init(pin);
    if (adc == NULL) {
        fprintf(stderr, "[-] Cannot init pin %d\n", pin);
        return 1;
    }

    R0 = CalibrateR0(&adc,sensor,gas);

    fprintf(stdout, "Sensor Calibrated R0:%f\n", R0);

    while (1) {
	
	sleep(TIMEOUT);
        
	sensor_volt = mraa_aio_read_float(adc)*VOLTS;
	sensorValue = (VOLTS-sensor_volt)/sensor_volt;

	double ppm = CalcPPM(sensorValue/R0, sensor, gas);        

	fprintf(stdout, "Pin A%d level %f [V] \n", pin, sensor_volt);
        fprintf(stdout, "Pin A%d ratio %f [RS/R0] \n", pin, sensorValue/R0);
	fprintf(stdout, "ppm : %f \n", ppm);

#ifdef CLOUD  
	sprintf(json," {\"n\":\"gass\", \"v\":%f }\n ",sensorValue/R0);
	fprintf(stdout, "json inspect : %s \n",json);
        sendto(sockfd,json,strlen(json),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
#endif
    }

   	
    

    mraa_aio_close(adc);

    return 0;
}



