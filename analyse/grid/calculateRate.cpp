#include <iostream>
#include <fstream>

#define DIVIDE_LAT_TO 10
#define DIVIDE_LNG_TO 9

using namespace std;

// [
// [2,3],
// [3,4]
// ]

int main() {
    double loseRate[DIVIDE_LAT_TO][DIVIDE_LNG_TO];
    int totalCount[DIVIDE_LAT_TO][DIVIDE_LNG_TO], CRCBADCount[DIVIDE_LAT_TO][DIVIDE_LNG_TO];
    ifstream input("accu.txt");
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            input >> totalCount[i][j];
        }
    }
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            input >> CRCBADCount[i][j];
        }
    }
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            loseRate[i][j] = (totalCount[i][j] == 0) ? 0 : (double)CRCBADCount[i][j] / (double)totalCount[i][j];
        }
    }

    ofstream output("array.txt");
    output << '[' << endl;
    for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
        output << '[';
        for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
            output << loseRate[i][j];
            if (j != DIVIDE_LNG_TO - 1) {
                output << ",";
            }
        }
        output << "]";
        if (i != DIVIDE_LAT_TO - 1) {
            output << ",";
        }
    }
    output << "]";
    output.close();
    input.close();
    return 0;
}