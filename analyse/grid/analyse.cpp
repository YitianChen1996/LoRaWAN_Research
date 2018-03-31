#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <cmath>
#include <vector>
#include <unordered_set>

#define earthRadiusKm 6371.0
#define CAMPUS_LAT_MIN 42.082444
#define CAMPUS_LAT_MAX 42.092444
#define CAMPUS_LNG_MIN (-75.980282)
#define CAMPUS_LNG_MAX (-75.962282)
#define DIVIDE_LAT_TO 10
#define DIVIDE_LNG_TO 9

using namespace std;

double deg2rad(double deg) {
    return (deg * M_PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
    return (rad * 180 / M_PI);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r) / 2);
    v = sin((lon2r - lon1r) / 2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

struct LoRaPacket {
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

unordered_set<int> set_busNum;

/*********other variables go here********/

struct CRCBAD_Segment {
    double beginLat, beginLng, endLat, endLng;
    int missCnt, busNum;
    bool isFindingEnd;

    explicit CRCBAD_Segment() : missCnt(0), isFindingEnd(false) {};
};

struct Prev_CRCOK {
    bool valid;
    double beginLat, beginLng;

    explicit Prev_CRCOK() : valid(false) {};
};

vector<double> crcbadBeginEnd;

/**************************************/

bool isValidDouble(const string &s) {
    for (int i = 0; i <= (int) s.length() - 1; i++) {
        if (!isdigit(s[i]) && s[i] != '.' && s[i] != '-') {
            return false;
        }
    }
    return true;
}

void setPacket(LoRaPacket &newPacket, const string &curSeg, int cnt) {
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

LoRaPacket parseLine(const string &s) {
    int p1 = 0, p2 = 0, cnt = 0;
    LoRaPacket newPacket;
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

bool isIncampus(double latitude, double longitude) {
    return (latitude >= CAMPUS_LAT_MIN && latitude <= CAMPUS_LAT_MAX &&
            longitude >= CAMPUS_LNG_MIN && longitude <= CAMPUS_LNG_MAX);
}

void init() {
    set_busNum.insert(747);
    set_busNum.insert(748);
    set_busNum.insert(762);
    set_busNum.insert(763);

    /********other initialise********/

    /********************************/
}

bool isValidPacket(const LoRaPacket &packet) {
    return (set_busNum.find(packet.busnum) != set_busNum.end());
}

/**********other functions go here********/

int getIndexLatitude(double latitude) {
    int diff = (int) ((42.092444 - latitude) * 1000000.0);
    int index = diff / (10000 / DIVIDE_LAT_TO);
    return (index >= 0 && index <= DIVIDE_LAT_TO - 1) ? index : -1;
}

int getIndexLongitude(double longitude) {
    int diff = (int) ((longitude - (-75.980282)) * 1000000.0);
    int index = diff / ((10000 / DIVIDE_LAT_TO) * 2);
    return (index >= 0 && index <= DIVIDE_LNG_TO - 1) ? index : -1;
}

int getArrayIndex(int busNum) {
    switch (busNum) {
        case 747: { return 0; }
        case 748: { return 1; }
        case 762: { return 2; }
        case 763: { return 3; }
        default: { return -1; }
    }
}

/*****************************************/

int main(int argc, char **argv) {
    string s;
    ifstream inputFile(argv[1]);
    ofstream outputFile("output.csv");

    init();

    /**********analyse variables go here**********/

    int crcok = 0, num748 = 0;

    int totalCount[DIVIDE_LAT_TO][DIVIDE_LNG_TO], CRCBADCount[DIVIDE_LAT_TO][DIVIDE_LNG_TO];
    ifstream accuinput("accu.txt");

    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            accuinput >> totalCount[i][j];
        }
    }

    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            accuinput >> CRCBADCount[i][j];
        }
    }

    accuinput.close();

    CRCBAD_Segment crcbad_segments[4];
    Prev_CRCOK prev_crcok[4];
    vector<CRCBAD_Segment> segments;
    crcbad_segments[0].busNum = 747;
    crcbad_segments[1].busNum = 748;
    crcbad_segments[2].busNum = 762;
    crcbad_segments[3].busNum = 763;

    /*********************************************/

    if (!inputFile) { cout << "Open input file failed" << endl; }
    if (!outputFile) { cout << "Open output file failed" << endl; }

    outputFile << "\"latitude\",\"longitude\"" << endl;

    getline(inputFile, s);
    while (getline(inputFile, s)) {
        LoRaPacket curPacket;
//        cout << s << endl;
        curPacket = parseLine(s);

        /*********select code goes here**********/

        if (!isValidPacket(curPacket) || !isIncampus(curPacket.latitude, curPacket.longitude)) {
            continue;
        }
        int arrayIndex = getArrayIndex(curPacket.busnum);
        if (curPacket.status) {
            if (crcbad_segments[arrayIndex].isFindingEnd) {
                crcbad_segments[arrayIndex].endLat = curPacket.latitude;
                crcbad_segments[arrayIndex].endLng = curPacket.longitude;
                crcbad_segments[arrayIndex].isFindingEnd = false;
                segments.push_back(crcbad_segments[arrayIndex]);
                crcbad_segments[arrayIndex].missCnt = 1;
            }
            int row = getIndexLatitude(curPacket.latitude);
            int col = getIndexLongitude(curPacket.longitude);
//            cout << row << " " << col << endl;
            if (row == -1 || col == -1) {
                continue;
            }
            crcok++;
            totalCount[row][col]++;

            prev_crcok[arrayIndex].valid = true;
            prev_crcok[arrayIndex].beginLat = curPacket.latitude;
            prev_crcok[arrayIndex].beginLng = curPacket.longitude;
        } else {
            if (crcbad_segments[arrayIndex].isFindingEnd) {
                crcbad_segments[arrayIndex].missCnt++;
            } else {
                if (prev_crcok[arrayIndex].valid) {
                    crcbad_segments[arrayIndex].isFindingEnd = true;
                    crcbad_segments[arrayIndex].beginLat = prev_crcok[arrayIndex].beginLat;
                    crcbad_segments[arrayIndex].beginLng = prev_crcok[arrayIndex].beginLng;
                }
            }
        }

        /****************************************/
    }

    /********print result here*********/

    for (int i = 0; i <= (int) segments.size() - 1; i++) {
        if (!segments[i].isFindingEnd) {
            double latDiff, lngDiff;
            latDiff = (segments[i].beginLat - segments[i].endLat) / (double) (segments[i].missCnt + 1);
            lngDiff = (segments[i].beginLng - segments[i].endLng) / (double) (segments[i].missCnt + 1);
            for (int j = 1; j <= segments[i].missCnt; j++) {
                double estimatedLat, estimatedLng;
                estimatedLat = segments[i].endLat + latDiff * j;
                estimatedLng = segments[i].endLng + lngDiff * j;
                int row = getIndexLatitude(estimatedLat);
                int col = getIndexLongitude(estimatedLng);
                if (row == -1 || col == -1) {
                    continue;
                }
                CRCBADCount[row][col]++;
                totalCount[row][col]++;
            }
//            crcbadBeginEnd.push_back(segments[i].beginLat);
//            crcbadBeginEnd.push_back(segments[i].beginLng);
//            crcbadBeginEnd.push_back(segments[i].endLat);
//            crcbadBeginEnd.push_back(segments[i].endLng);
        }
    }
    ofstream accuOutput("accu.txt");

    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            accuOutput << totalCount[i][j] << " ";
        }
        accuOutput << endl;
    }
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            accuOutput << CRCBADCount[i][j] << " ";
        }
        accuOutput << endl;
    }

    accuOutput.close();

    accuOutput.open("accu2.txt", ios::app);
    for (int i = 0; i <= (int) crcbadBeginEnd.size() - 1; i++) {
        accuOutput << crcbadBeginEnd[i] << " ";
    }
    /**********************************/

    cout << crcok << endl;

    inputFile.close();
    outputFile.close();
    return 0;
}