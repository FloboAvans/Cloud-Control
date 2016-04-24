#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mraa/aio.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

float CalibrateR0(mraa_aio_context* aio_pin,int sesnorType)
{
    	float sensor_volt = 0;
	float sensorValue = 0;
    	float RS_air = 0; // Get the value of RS via in a clear air
    	float R0 = 0; // Get the value of R0 via in LPG

	
	for(int x = 0 ; x < 100 ; x++)
	{
        	sensorValue = sensorValue + mraa_aio_read_float(*aio_pin)*5.0;
	}
	sensorValue = sensorValue/100.0;

	//printf("ahh: %f",sensorValue);

	sensor_volt = sensorValue;
	RS_air = (5.0-sensor_volt)/sensor_volt; // omit *RL
	R0 = RS_air/9.9; // The ratio of RS/R0 is 9.9 in LPG gas

	return R0;
}

int main(int argc, char** argv)
{
    mraa_aio_context adc;
    //uint16_t value;
 
    int sockfd;
    struct sockaddr_in servaddr;
    char json[100];
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(41234);

    int type = atoi(argv[1]);    
    int pin = atoi(argv[2]);
    
    float sensor_volt;
    //float RS_air; // Get the value of RS via in a clear air
    float R0; // Get the value of R0 via in LPG
    float sensorValue;

    // Init Pin
    adc = mraa_aio_init(pin);
    if (adc == NULL) {
        fprintf(stderr, "[-] Cannot init pin %d\n", pin);
        return (1);
    }

    R0 = CalibrateR0(&adc,type);

    fprintf(stdout, "Pin Calibrated R0:%f\n", R0);
  
    while (1) {
	sleep(3);
        sensor_volt = mraa_aio_read_float(adc)*5.0;
	sensorValue = (5.0-sensor_volt)/sensor_volt;
        fprintf(stdout, "Pin A%d level %f [V] \n", pin, sensor_volt);
        fprintf(stdout, "Pin A%d ratio %f [RS/R0] \n", pin, sensorValue/R0);
        
	sprintf(json," {\"n\":\"gass\", \"v\":%f }\n ",sensorValue/R0);
	fprintf(stdout, "json inspect : %s \n",json);
        sendto(sockfd,json,strlen(json),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
    }

   	
    

    mraa_aio_close(adc);

    return 0;
}



