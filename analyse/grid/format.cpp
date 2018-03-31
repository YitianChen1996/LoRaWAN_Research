#include <iostream>
#include <fstream>
#include <iomanip>

#define DIVIDE_LAT_TO 10
#define DIVIDE_LNG_TO 9

using namespace std;

int main() {
    int totalCnt[DIVIDE_LAT_TO][DIVIDE_LNG_TO], careCnt[DIVIDE_LAT_TO][DIVIDE_LNG_TO];

    ifstream inputFile("accu.txt");
    ofstream outputFile("accu2.txt");
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            inputFile >> totalCnt[i][j];
        }
    }
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            inputFile >> careCnt[i][j];
        }
    }

    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            outputFile << setw(7) << totalCnt[i][j];
        }
        outputFile << endl;
    }

    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            outputFile << setw(7) << careCnt[i][j];
        }
        outputFile << endl;
    }

    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            outputFile << setw(7) << fixed << setprecision(3) << (double)careCnt[i][j] / (double)totalCnt[i][j];
        }
        outputFile << endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;   
}