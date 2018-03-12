#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

struct Packet {
    unsigned long lineNum; //cnt = 0
    string gatewayID; //1
    string nodeMAC; //2
    string UTCtimestamp; //3
    uint64_t uscount; //4
    uint64_t freqency; //5
    short RFchain; //6
    short RXchain; //7
    bool status; //8
    unsigned long size; //9
    string modulation; //10
    unsigned long bandwidth; //11
    string datarate; //12
    string coderate; //13
    int RSSI; //14
    double SNR; //15
    string payload; //16
    int busnum, packetnum; //17, 18
    double latitude, longitude, speed, bear; //19,20,21,22
};

bool isValidDouble(const string &s) {
    for (int i = 0; i <= (int) s.length() - 1; i++) {
        if (!isdigit(s[i]) && s[i] != '.' && s[i] != '-') {
            return false;
        }
    }
    return true;
}

void setPacket(Packet &newPacket, const string &curSeg, int cnt) {
    switch (cnt) {
        case 0: {
            newPacket.lineNum = (unsigned) stoi(curSeg, nullptr, 10);
            break;
        }
        case 1: {
            newPacket.gatewayID = curSeg;
            break;
        }
        case 2: {
            newPacket.nodeMAC = curSeg;
            break;
        }
        case 3: {
            newPacket.UTCtimestamp = curSeg;
            break;
        }
        case 4: {
            newPacket.uscount = (unsigned) stoll(curSeg, nullptr, 10);
            break;
        }
        case 5: {
            newPacket.freqency = (unsigned) stoll(curSeg, nullptr, 10);
            break;
        }
        case 6: {
            newPacket.RFchain = (short) stoi(curSeg, nullptr, 10);
            break;
        }
        case 7: {
            newPacket.RXchain = (short) stoi(curSeg, nullptr, 10);
            break;
        }
        case 8: {
            newPacket.status = (curSeg == "\"CRC_OK\" ");
            break;
        }
        case 9: {
            newPacket.size = (unsigned) stoi(curSeg, nullptr, 10);
            break;
        }
        case 10: {
            newPacket.modulation = curSeg;
            break;
        }
        case 11: {
            newPacket.bandwidth = (unsigned) stoi(curSeg, nullptr, 10);
            break;
        }
        case 12: {
            newPacket.datarate = curSeg;
            break;
        }
        case 13: {
            newPacket.coderate = curSeg;
            break;
        }
        case 14: {
            newPacket.RSSI = stoi(curSeg, nullptr, 10);
            break;
        }
        case 15: {
            newPacket.SNR = stod(curSeg);
            break;
        }
        case 16: {
            newPacket.payload = curSeg;
            break;
        }
        case 17: {
            newPacket.busnum = stoi(curSeg, nullptr, 10);
            break;
        }
        case 18: {
            newPacket.packetnum = stoi(curSeg, nullptr, 10);
            break;
        }
        case 19: {
            newPacket.latitude = isValidDouble(curSeg) ? stod(curSeg) : 0;
            break;
        }
        case 20: {
            newPacket.longitude = isValidDouble(curSeg) ? stod(curSeg) : 0;
            break;
        }
        case 21: {
            newPacket.speed = isValidDouble(curSeg) ? stod(curSeg) : 0;
            break;
        }
        case 22: {
            newPacket.bear = isValidDouble(curSeg) ? stod(curSeg) : 0;
            break;
        }
        default: {}
    }
}

Packet parseLine(const string &s) {
    int p1 = 0, p2 = 0, cnt = 0;
    Packet newPacket;
    while (p2 < s.length()) {
        while (p2 < s.length() && s[p2] != ',') {
            p2++;
        }
        if (p2 == s.length()) {
            setPacket(newPacket, s.substr((unsigned) p1, (unsigned) (p2 - p1 + 1)), cnt);
            break;
        }
        string curSeg = s.substr((unsigned) p1, (unsigned) (p2 - p1));
        setPacket(newPacket, curSeg, cnt);
        cnt++;
        p1 = p2 + 1;
        p2 = p1;
    }
    return newPacket;
}

bool isGPSReasonable(double latitude, double longitude) {
    return (latitude >= 42.0 && latitude <= 43.0 && longitude >= -76.0 && longitude <= -75.0);
}

int main(int argc, char **argv) {
    string s;
    ifstream inputFile(argv[1]);

    if (!inputFile) { cout << "Open file failed" << endl; }

    getline(inputFile, s);
    while (getline(inputFile, s)) {
        Packet curPacket;
//        cout << s << endl;
        curPacket = parseLine(s);

        /*********select code goes here**********/
        if (!curPacket.status && isGPSReasonable(curPacket.latitude, curPacket.longitude)) {
            cout << curPacket.latitude << " " << curPacket.longitude;
        }


        /****************************************/
    }
    inputFile.close();
    return 0;
}
