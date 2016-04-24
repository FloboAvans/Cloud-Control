/* DHT library 

MIT license
written by Adafruit Industries
*/
#include "DHT.h"

#include <cmath>
#include <iostream>

#include "grove.h"
#include "mraa.h"

// BEGIN: some stubs
#define HIGH 1
#define LOW 0
#include <chrono>
#include <thread>
template <typename T>
void delay(T d)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(d));
}
template <typename T>
void delayMicroseconds(T d) {
	std::this_thread::sleep_for(std::chrono::microseconds(d));

}
unsigned long millis()
{
    static std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
// END

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) {
  _pin = pin;
  _type = type;
  _count = count;
  firstreading = true;
}

void DHT::begin(void) {
  // set up the pins!
  //pinMode(_pin, INPUT);
  //digitalWrite(_pin, HIGH);
  //
  //NOTE: I simply instantiate the pin object in the read(), so it's a bit hardcoded.

  _lastreadtime = 0;
}

//boolean S == Scale.  True == Farenheit; False == Celcius
float DHT::readTemperature(bool S) {
  float f;

  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[2];
      if(S)
      	f = convertCtoF(f);
      	
      return f;
    case DHT22:
    case DHT21:
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f /= 10;
      if (data[2] & 0x80)
	f *= -1;
      if(S)
	f = convertCtoF(f);

      return f;
    }
  }
  //Serial.print("Read fail");
  return NAN;
}

float DHT::convertCtoF(float c) {
	return c * 9 / 5 + 32;
}

float DHT::readHumidity(void) {
  float f;
  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[0];
      return f;
    case DHT22:
    case DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f /= 10;
      return f;
    }
  }
  //Serial.print("Read fail");
  return NAN;
}

bool DHT::read(void) {
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  unsigned long currenttime;

  // pull the pin high and wait 250 milliseconds
  mraa::Gpio pin(0, true, false);
  //digitalWrite(_pin, HIGH);
  pin.write(1);
  delay(250);

  currenttime = millis();
  if (currenttime < _lastreadtime) {
    // ie there was a rollover
    _lastreadtime = 0;
  }
  if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
    return true; // return last correct measurement
    //delay(2000 - (currenttime - _lastreadtime));
  }
  firstreading = false;
  /*
    Serial.print("Currtime: "); Serial.print(currenttime);
    Serial.print(" Lasttime: "); Serial.print(_lastreadtime);
  */
  _lastreadtime = millis();

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
  // now pull it low for ~20 milliseconds
  //pinMode(_pin, OUTPUT);
  mraa::Result response = pin.dir(mraa::DIR_OUT);
  if (response != mraa::SUCCESS) {
      //mraa::printError(response);
      //return 1;
  }
  //digitalWrite(_pin, LOW);
  pin.write(0);
  delay(20);
  //TODO:: cli();
  //digitalWrite(_pin, HIGH);
  pin.write(1);
  delayMicroseconds(40);
  //pinMode(_pin, INPUT);
  mraa::Result response2 = pin.dir(mraa::DIR_IN);

  // read in timings
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    //while (digitalRead(_pin) == laststate) {
    while (pin.read() == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    //laststate = digitalRead(_pin);
    laststate = pin.read();

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > _count)
        data[j/8] |= 1;
      j++;
    }
  }

  //TODO::sei();
  
  /*
  Serial.println(j, DEC);
  Serial.print(data[0], HEX); Serial.print(", ");
  Serial.print(data[1], HEX); Serial.print(", ");
  Serial.print(data[2], HEX); Serial.print(", ");
  Serial.print(data[3], HEX); Serial.print(", ");
  Serial.print(data[4], HEX); Serial.print(" =? ");
  Serial.println(data[0] + data[1] + data[2] + data[3], HEX);
  */

  // check we read 40 bits and that the checksum matches
  if ((j >= 40) && 
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
    return true;
  }
  

  return false;

}
