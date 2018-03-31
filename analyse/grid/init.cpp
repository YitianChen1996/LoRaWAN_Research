#include <iostream>
#include <fstream>

#define DIVIDE_LAT_TO 10
#define DIVIDE_LNG_TO 9

using namespace std;

int main() {
    ofstream accuOutput("accu.txt");
	for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
		for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
			accuOutput << 0 << " ";
		}
		accuOutput << endl;
	}
	for (int i = 0; i <= DIVIDE_LAT_TO - 1; i++) {
		for (int j = 0; j <= DIVIDE_LNG_TO - 1; j++) {
			accuOutput << 0 << " ";
		}
		accuOutput << endl;
	}
    accuOutput.close();
    return 0;
//    ifstream input("./accu.txt");
//    if (!input) {
//        cout << "open failed" << endl;
//        return 0;
//    }
//    int x, accu = 0;
//    while (input >> x) {
//        accu += x;
//    }
//    cout << accu << endl;
}



