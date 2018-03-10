//aim of this program:
//1. Take out the duplicated lines (same bus number and same packet number)
//2. Parse and add the latitude, longitude, speed, bear of the payload

#include <iostream>
#include <fstream>
#include <cstring>

#define NORMAL_PACKET_SIZE 82

using namespace std;

unsigned short changChartoShort(char c) {
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            return 0x0000;
    }
}

double changeIEEEtoDouble(unsigned char *IEEE) {
    double *pd, module;
    unsigned char *pc;
    pd = &module;
    pc = (unsigned char *) &module;
    memset(pd, 0, 8 * sizeof(unsigned char));
    for (int i = 0; i <= 7; i++) {
        *pc = 0;
        *pc = (unsigned char) (changChartoShort(IEEE[2 * i]) << 4);
        *pc = *pc + (unsigned char) changChartoShort(IEEE[2 * i + 1]);
        pc++;
    }
    return *pd;
}

void parsePayload(char *s, double &latitude, double &longitude, double &speed, double &bear) {
    char *payload;
    unsigned char latitudestring[32], longitudestring[32], speedstring[32], bearstring[32];
    int busNum;

    payload = strchr(s, '-') + 1;
//    printf("%s\n", payload);
    memset(latitudestring, 0, sizeof(latitudestring));
    memset(longitudestring, 0, sizeof(longitudestring));
    memset(speedstring, 0, sizeof(speedstring));
    memset(bearstring, 0, sizeof(bearstring));
    strncat((char *) latitudestring, payload, 8);
    payload = strchr(payload, '-') + 1;
    strncat((char *) latitudestring, payload, 8);
    latitude = changeIEEEtoDouble(latitudestring);

    payload = strchr(payload, '-') + 1;
    strncat((char *) longitudestring, payload, 8);
    payload = strchr(payload, '-') + 1;
    strncat((char *) longitudestring, payload, 8);
    longitude = changeIEEEtoDouble(longitudestring);

    payload = strchr(payload, '-') + 1;
    strncat((char *) speedstring, payload, 8);
    payload = strchr(payload, '-') + 1;
    strncat((char *) speedstring, payload, 8);
    speed = changeIEEEtoDouble(speedstring);

    payload = strchr(payload, '-') + 1;
    strncat((char *) bearstring, payload, 8);
    payload = strchr(payload, '-') + 1;
    strncat((char *) bearstring, payload, 8);
    bear = changeIEEEtoDouble(bearstring);
    if (!(latitude >= 42.0 && latitude <= 43.0 && longitude >= -76.0 && longitude <= -75.0)) {
        latitude = 0;
        longitude = 0;
    }
}

string getPayload(const string &s) {
    int ccnt = 0, i;
    for (i = 0; i <= s.length() - 1; i++) {
        if (s[i] == ',') {
            ccnt++;
        }
        if (ccnt == 15) {
            break;
        }
    }
    return (i == s.length()) ? s : s.substr((unsigned) i + 1, s.length() - 1 - i);
}

bool hasBusandPacketNum(const string &payload, int &busNum, int &packetNum) {
    if (payload.length() != NORMAL_PACKET_SIZE) {
        return false;
    }
    for (int i = 1; i <= 8; i++) {
        if (!isdigit(payload[i])) {
            return false;
        }
    }
    busNum = stoi(payload.substr(1, 4), nullptr, 10);
    packetNum = stoi(payload.substr(5, 8), nullptr, 10);
    return true;
}

int main(int argc, char **argv) {
    string s;
    ifstream inputFile(argv[1]);
    ofstream outputFile("new_" + (string) argv[1]);

    unsigned long lineNum = 0;
    cout << argv[1] << endl;
    if (!inputFile) {
        cout << "file not good" << endl;
    }

    int busNum, packetNum, prevbusNum = 0, prevpacketNum = 0;
    while (getline(inputFile, s)) {
        cout << s << endl;
        if (lineNum == 0) {
            outputFile << "\"ID\",";
            outputFile << s;
            outputFile << "\"latitude\"" << "," << "\"longitude\"" << "\"speed\"" << "\"bear\"" << endl;
            lineNum++;
        } else {
            cout << getPayload(s) << endl;
            string payload = getPayload(s);
            cout << payload.length() << endl;
            double latitude = 0, longitude = 0, speed = 0, bear = 0;
            if (hasBusandPacketNum(payload, busNum, packetNum) && payload.length() == NORMAL_PACKET_SIZE) {
                if (busNum != prevbusNum || packetNum != prevpacketNum) {
                    char cstr[120];
                    memset(cstr, 0, sizeof(cstr));
                    strcpy(cstr, payload.c_str());
                    parsePayload(cstr, latitude, longitude, speed, bear);
                    outputFile << lineNum << ',';
                    outputFile << s << "," << latitude << "," << longitude << "," << speed << "," << bear << endl;
                    lineNum++;
                }
                prevbusNum = busNum;
                prevpacketNum = packetNum;
            } else {
                outputFile << lineNum << ',';
                outputFile << s << "," << latitude << "," << longitude << "," << speed << "," << bear << endl;
                lineNum++;
            }
        }
    }
    inputFile.close();
    outputFile.close();
    return 0;
}