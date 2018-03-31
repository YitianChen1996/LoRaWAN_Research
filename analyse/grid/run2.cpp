#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>

using namespace std;

string databasePath = "/mnt/c/Users/64419/Desktop/Termination_Project/Analyses/database/";

int main() {
    ifstream inputFile(databasePath + "csv.txt");

    string s;
    while (getline(inputFile, s)) {
	cout << s <<endl;
        if (strstr(s.c_str(), ".csv") != nullptr) {
            system(("./analyse " + databasePath + s).c_str());
        }
    }
    inputFile.close();
}
