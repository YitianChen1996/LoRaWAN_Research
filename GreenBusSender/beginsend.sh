fuser -k /dev/ttyUSB0
tput reset > /dev/ttyUSB0
./moveFile &
./AdafruitGPS_main &
sudo ./GreenBusSender.cpp_exe
