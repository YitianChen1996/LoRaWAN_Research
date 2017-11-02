#include "GPSClass.h"

char dev[30]="/dev/ttyUSB0";
int fileNum;

int main(){
	char buffer[520];
	Adafruit_GPS_CPP gps(dev, 1);
	int cnt = 0;
	gps.standby();
	usleep(1000000);
	tcflush(gps.serial, TCIOFLUSH);
	gps.start();
	usleep(3000000);
	gps.showAntenna();
	memset(buffer,0,sizeof(buffer));
	fileNum = 0;
	while(1){
		char tempbuffer[4];
		if (cnt <= 512){
			memset(tempbuffer,0,sizeof(tempbuffer));
			gps.serial_read((void*)tempbuffer,5);
			strcat(buffer,tempbuffer);
			cnt+=5;
			buffer[cnt] = '\0';
		}else{	
		    fileNum++;
		    if (fileNum == 100) {
			    fileNum = 0;
		    }
		    tcflush(gps.serial, TCIOFLUSH);
		    //buffer[511]='\0';
		    //printf("recieved length: %d\n", strlen(buffer));
		    bool ret;
		    //printf("recieve buffer:\n%s\n", buffer);
		    ret = gps.parse(buffer);
		    //printf("%d\n", gps.fix);
		    FILE *file;
		    file = fopen("AdafruitGPSdata.txt", "w");
		    if (file == NULL){
			printf("AdafruitGPS_main: open file failed\n");
		    	usleep(123456);
			continue;
		    }
			//printf("%d %lf %lf %lf %lf\n", 
			//	gps.latitudeDegrees, gps.longitudeDegrees, gps.speed, gps.angle);
		    fprintf(file, "%d %lf %lf %lf %lf\n", 
		    fileNum, gps.latitudeDegrees, gps.longitudeDegrees, gps.speed, gps.angle);
	            fclose(file);
		    if (ret == true){
		       	if (gps.fix){
				printf("AdafruitGPS_main: got GPS data from Adafruit, write it to text file\n");
			} else {
			    printf("AdafruitGPS_main: no fix yet!\n");
		        }
		    } else {
			printf("AdafruitGPS_main: data is not ready\n");
		    }
		    usleep(5000000);
		    memset(buffer,'\0',sizeof(buffer));
		    cnt = 0;
	    }
	}
}   
