#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <tgmath.h>

struct GPSData{
	double latitude, longitude, altitude, speed;
};

class Adafruit_GPS_CPP{
	
private:
	u_int8_t parseHex(char c);
	u_int8_t hour, minute, seconds, year, month, day;
    u_int16_t milliseconds;
    // Floating point latitude and longitude value in degrees.
    
    // Fixed point latitude and longitude value with degrees stored in units of 1/100000 degrees,
    // and minutes stored in units of 1/100000 degrees.  See pull #13 for more details:
    //   https://github.com/adafruit/Adafruit-GPS-Library/pull/13
    int32_t latitude_fixed, longitude_fixed;
    float geoidheight, altitude;
    char lat, lon, mag;
    u_int8_t fixquality, satellites;
	
public:
    	int serial;
	bool fix;
	float latitude, longitude;
	float latitudeDegrees, longitudeDegrees;
	float speed, angle, magvariation, HDOP;
	
	Adafruit_GPS_CPP(char* name, int block);//construcor
	int start();
	int serial_read (void* buffer, int length);
	int serial_write (void* buffer, int length);
	int standby();
	bool parse(char* rawData);
	~Adafruit_GPS_CPP();
};
