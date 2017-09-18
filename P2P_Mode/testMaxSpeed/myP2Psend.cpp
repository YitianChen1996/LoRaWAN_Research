#include "arduPiLoRaWAN.h"
#include <string.h>
#include <ctime>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <fstream>

uint8_t error;
uint8_t power = 15;
uint32_t frequency = 900000000;
char spreading_factor[] = "sf12";
char coding_rate[] = "4/5";
uint16_t bandwidth = 500;
char crc_mode[] = "on";
char data[510];
uint8_t sock = SOCKET0;
int cnt=0, datalen = 50, delayt, timecnt = 0;
//clock_t tic,toc;
//auto tic,toc;
bool autoincre = false;

using namespace std;
using namespace chrono;

void generateData1(int len){//fill in the length of a packet
	char temp[10];
	snprintf(temp,sizeof(temp),"%d",len);
	int i, slen = strlen(temp);
	//printf("temp:%s",temp);
	memset(data,'\0',sizeof(data));
	for (i = 0;i <= len  - slen -2;i++){
		data[i] = '0';
	}
	data[len-slen-1]='A';
	strcpy(data + i + 1,temp);
	printf("data:%s\nlength:%d\n",data,strlen(data));
}

void generateData2(int len, int cnt){//fill in the length of a packet
	char temp[10];
	snprintf(temp,sizeof(temp),"%d",cnt);
	int i, slen = strlen(temp);
	//printf("temp:%s",temp);
	memset(data,'\0',sizeof(data));
	for (i = 0;i <= len  - slen -2;i++){
		data[i] = '0';
	}
	data[len-slen-1]='A';
	strcpy(data + i + 1,temp);
	printf("data:%s",data);
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

void loop() 
{
	printf("No. %d\n", cnt);
	if (autoincre){
		generateData1(datalen);
	}else{
		generateData2(datalen,cnt);
	}
  	// Send packet
  	error = LoRaWAN.sendRadio(data);
  
  	// Check status
  	if (error == 0){
		//printf("datalen:%d", datalen);
    		printf("--> Packet sent OK\n");
		//datalen++;
		cnt++;
  	}else {
    		printf("Error waiting for packets. error = %d\n", error);
	      return;	
  	}
	delay(delayt);
	
	if (autoincre){
		datalen++;
	}

}

/*
 * arguments1:the initial length of send data
 * argu2:the send time out
 * argu3:autoincrement length of data
 * argu4:time count every n packets,0 show no time count*/

int main(int argc, char* argv[]){
	datalen=atoi(argv[1]);
	delayt=atoi(argv[2]);
	autoincre = atoi(argv[3]);
	timecnt = atoi(argv[4]);
	strcpy(spreading_factor,argv[5]);
	bandwidth = atoi(argv[6]);
	ofstream datafile;
	datafile.open("data.txt",ios::app);
	if (!datafile.is_open()){
		cout<<"open file failed"<<endl;
	}
	setup();
	auto tic = system_clock::now();
	while(timecnt--){
		loop();
	}
        auto toc = system_clock::now();
	auto duration = duration_cast<microseconds>(toc-tic);
	delay(1000);
	LoRaWAN.sendRadio("AA");
	//datafile<<"packetlen:"<<argv[1]<<endl;
	//datafile<<"delay time:"<<argv[2]<<endl;
	//datafile<<"autoinc:"<<argv[3]<<endl;
	//datafile<<"num of packets:"<<argv[4]<<endl;
	//datafile<<"sf:"<<argv[5]<<endl;
	datafile<<double(duration.count())*microseconds::period::num/microseconds::period::den<<endl;
	//datafile<<endl;
	datafile.close();
	return 0;
}

