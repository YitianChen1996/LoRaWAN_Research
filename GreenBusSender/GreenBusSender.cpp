#include <stdio.h>
#include "arduPiLoRaWAN.h"
#include <string.h>
#include <unistd.h>

using namespace std;

const char dataFileLoc_fopen[150] = "/run/user/1000/gvfs/mtp:host=%5Busb%3A001%2C007%5D/Internal shared storage/data.txt";

uint8_t error;
uint8_t power = 15;
uint32_t frequency = 923300000;
char spreading_factor[] = "sf12";
char coding_rate[] = "4/5";
uint16_t bandwidth = 125;
char crc_mode[] = "on";
FILE *lockFile, *dataFile;
uint8_t sock = SOCKET0;

void moveToLocal(){
	printf("run mv\n");
	//system("cp phoneData.txt ../");
	//system("cp /run/user/1000/gvfs/mtp:host=%5Busb%3A001%2C011%5D/Internal\\ shared\\ storage/Download/data.txt ./");
}

uint8_t radioModuleSetup(){
	uint8_t status = 0;
	uint8_t e = 0;

	e = LoRaWAN.ON(sock);
  	if (e == 0){
    		printf("1. Switch ON OK\n");     
  	}else {
    		printf("1. Switch ON error = %d\n",e); 
    		status = 1;
	}
  	printf("-------------------------------------------------------\n");

	//disable LoRaWAN mode and enable  P2P mode
	e = LoRaWAN.macPause();

	if (e == 0){
    		printf("2. P2P mode enabled OK\n");
  	}else {
    		printf("2. Enable P2P mode error = %d\n", e);
    		status = 1;
	}
	printf("-------------------------------------------------------\n");

	//set power level,different level correspond to different frequency
	e = LoRaWAN.setRadioPower(power);
	if (e == 0){
		printf("3.1. Set Radio Power OK\n");
	}else {
		printf("3.1. Set Radio Power error = %d\n", e);
		status = 1;
	}

	//check power level
	e = LoRaWAN.getRadioPower();
  	if (e == 0) {
    		printf("3.2. Get Radio Power OK. "); 
    		printf("Power: %d\n",LoRaWAN._radioPower);
  	}else {
    		printf("3.2. Get Radio Power error = %d\n", e); 
    		status = 1;
  	}
  	printf("-------------------------------------------------------\n");

  	// Set frequency
  	e = LoRaWAN.setRadioFreq(frequency);
  	if (e == 0){
    		printf("4.1. Set Radio Frequency OK\n");
	}else {
    		printf("4.1. Set Radio Frequency error = %d\n", error);
    		status = 1;
  	}

  	// Get frequency
  	e = LoRaWAN.getRadioFreq();
	if (e == 0) {
    		printf("4.2. Get Radio Frequency OK. "); 
    		printf("Frequency: %d\n", LoRaWAN._radioFreq);
  	}else {
    		printf("4.2. Get Radio Frequency error = %d\n", e); 
    		status = 1;
  	}
  	printf("-------------------------------------------------------\n");

  	// Set SF
  	e = LoRaWAN.setRadioSF(spreading_factor);
	if (e == 0){
    		printf("5.1. Set Radio SF OK\n");
  	}else {
    		printf("5.1. Set Radio SF error = %e\n", e);
    		status = 1;
  	}

  	// Get SF
  	e = LoRaWAN.getRadioSF();
  	if (e == 0) {
    		printf("5.2. Get Radio SF OK. "); 
    		printf("Spreading Factor: %s\n", LoRaWAN._radioSF);
  	}else {
    		printf("5.2. Get Radio SF error = %d\n", e); 
    		status = 1;
  	}
  	printf("-------------------------------------------------------\n");

  	// Set CR
  	e = LoRaWAN.setRadioCR(coding_rate);
  	if (e == 0){
    		printf("6.1. Set Radio CR OK\n");
	}else {
    		printf("6.1. Set Radio CR error = %d\n", e);
    		status = 1;
  	}

  	// Get CR
  	e = LoRaWAN.getRadioCR();
  	if (e == 0) {
    		printf("6.2. Get Radio CR OK. "); 
    		printf("Coding Rate: %s\n",LoRaWAN._radioCR);
  	}else {
    		printf("6.2. Get Radio CR error = %d\n", e); 
    		status = 1;
  	}
  	printf("-------------------------------------------------------\n");

  	// Set BW
  	e = LoRaWAN.setRadioBW(bandwidth);
  	if (e == 0){
    		printf("7.1. Set Radio BW OK\n");
  	}else {
    		printf("7.1. Set Radio BW error = %d\n", e);
  	}

  	// Get BW
  	e = LoRaWAN.getRadioBW();
  	if (e == 0) {
    		printf("7.2. Get Radio BW OK. "); 
    		printf("Bandwidth: %u\n", LoRaWAN._radioBW);
  	}else {
    		printf("7.2. Get Radio BW error = %d\n", e); 
    		status = 1;
  	}
  	printf("-------------------------------------------------------\n");

	//enable or disable crc check
  	e = LoRaWAN.setRadioCRC(crc_mode);
  	if (e == 0){
    		printf("8.1. Set Radio CRC mode OK\n");
  	}else {
    		printf("8.1. Set Radio CRC mode error = %d\n", e);
    		status = 1;
  	}

  	// Get CRC
  	e = LoRaWAN.getRadioCRC();
  	if (e == 0){
    		printf("8.2. Get Radio CRC mode OK. "); 
    		printf("CRC status: %u\n",LoRaWAN._crcStatus);
  	}else {
    		printf("8.2. Get Radio CRC mode error = %e\n", e); 
    		status = 1;
  	}	
  	printf("-------------------------------------------------------\n");
	printf("preamble length: %d\n",LoRaWAN.getRadioPreamble());
	return status;
}

void setup(){
	printf("Radio P2P example - Sending packets\n\n");
	
	error=radioModuleSetup();

	if (error == 0) {
		printf("Module configured OK\n");     
	} else {
    		printf("Module configured ERROR\n");     
  	}
}

void init(){
	setup();
}

//mutex = 1 writer finished;
//mutex = 2 reader finished;

/*void readerUnlock(){
    lockFile = fopen(lockFileLoc,"w");
    fputc('2', lockFile);
    fclose(lockFile);
}

void readerLock(){
    while(1){
        lockFile = fopen(lockFileLoc,"r");
        char mutex;
        if (lockFile == NULL){
			printf("read file failed\n");
		}
        mutex = fgetc(lockFile);
        fclose(lockFile);
        if (mutex != '1'){
            printf("get lock failed\n");
            delay(500);
            continue;
        } else {
			printf("got the lock\n");
            lockFile = fopen(lockFileLoc,"w");
            fputc('1', lockFile);
            fclose(lockFile);
            break;
        }
    }
}*/

int sendbuff(char *buff){
	printf("%s\n", buff);
	error = LoRaWAN.sendRadio(buff);
	if (error == 0){
    	printf("--> Packet sent OK\n");
    	printf("content: %s\nlength: %d\n", buff, strlen(buff));
  	}else {
		printf("Error waiting for packets. error = %d\n", error);	
  	}
  	return error;
}

void changeDoubletoIEEE(double *dp, unsigned char *&ret) {
    unsigned char *cp;
    cp = (unsigned char *) dp;
    unsigned char temp[8];
    for (int i = 0; i <= 7; i++) {
        temp[i] = cp[i];
    }
    for (int i = 0; i <= 7; i++) {
        unsigned char tempprint[2];
        sprintf((char *) tempprint, "%02X", temp[i]);
        strcat((char *) ret, (char *) tempprint);
    }
}

void readAndSend(){
	unsigned char *buff;
	char GPSFileline[128], PhoneLine[128];
	int PhoneFileCount;
	buff = (unsigned char*)malloc(sizeof(char) * 140);
	FILE *file;
	while(1){
		double GPSlatitudeDegrees, GPSlongitudeDegrees, GPSspeed, GPSangle, Phonelatitude, Phonelongitude, Phonespeed, Phoneangle;
		file = fopen("AdafruitGPSdata.txt", "r");
		if (file == NULL) {
			printf("file open failed!\n");
			return;
		}
		memset(GPSFileline, 0, sizeof(GPSFileline));
		fgets(GPSFileline, sizeof(GPSFileline), file);
		fclose(file);
		sscanf(GPSFileline,"%lf %lf %lf %lf", &GPSlatitudeDegrees, &GPSlongitudeDegrees, &GPSspeed, &GPSangle);
		printf("GPS data: %lf %lf %lf %lf\n", GPSlatitudeDegrees, GPSlongitudeDegrees, GPSspeed, GPSangle);
		//moveToLocal();
		file = fopen("phoneData.txt", "r");
		memset(PhoneLine, 0, sizeof(PhoneLine));
		fgets(PhoneLine, sizeof(PhoneLine), file);
		fclose(file);
		sscanf(PhoneLine, "%d %lf %lf %lf %lf\n",&PhoneFileCount, &Phonelongitude, &Phonelatitude, &Phonespeed, &Phoneangle);
		printf("phone Data: %d %lf %lf %lf %lf\n",PhoneFileCount, Phonelatitude, Phonelongitude, Phonespeed, Phoneangle);
		memset(buff,0,sizeof(unsigned char) * 140);
		changeDoubletoIEEE(&GPSlatitudeDegrees, buff);
		changeDoubletoIEEE(&GPSlongitudeDegrees, buff);
		changeDoubletoIEEE(&GPSspeed, buff);
		changeDoubletoIEEE(&GPSangle, buff);
		changeDoubletoIEEE(&Phonelatitude, buff);
		changeDoubletoIEEE(&Phonelongitude, buff);
		changeDoubletoIEEE(&Phonespeed, buff);
		changeDoubletoIEEE(&Phoneangle, buff);
		int ret;
		ret = sendbuff((char*)buff);
		delay(5000);
	}
}

int main(){
	init();
	readAndSend();
	return 0;
}	
