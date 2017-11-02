all: AdafruitGPS_main moveFile GreenBusSender.cpp_exe
GreenBusSender.cpp_exe: GreenBusSender.cpp
	./cook.sh GreenBusSender.cpp
moveFile: moveFile.c
	gcc moveFile.c -o moveFile
AdafruitGPS_main: AdafruitGPS_main.cpp GPSClass.cpp
	g++ GPSClass.cpp AdafruitGPS_main.cpp -o AdafruitGPS_main
clean:
	rm -f AdafruitGPS_main moveFile GreenBusSender.cpp_exe	
