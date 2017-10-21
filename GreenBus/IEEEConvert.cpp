#include <iostream>
#include <string.h>

void changeDoubletoIEEE(double *dp, unsigned char *&ret) {
    unsigned char *cp;
    cp = (unsigned char *) dp;
    unsigned char temp[8];
    for (int i = 0; i <= 7; i++) {
        temp[i] = cp[i];
    }
    for (int i = 0; i <= 7; i++) {
        unsigned char tempprint[2];
        sprintf((char *) tempprint, "%02X", temp[i]);
        strcat((char *) ret, (char *) tempprint);
    }
}

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
        *pc = (unsigned char)(changChartoShort(IEEE[2 * i]) << 4);
        *pc = *pc + (unsigned char) changChartoShort(IEEE[2 * i + 1]);
        pc++;
    }
    pc = (unsigned char *) &module;
    return *pd;
}

int main() {
    double a = -75.1235432;
    unsigned char *ac;
    ac = (unsigned char *) malloc(16 * sizeof(char));
    memset(ac, 0, 16 * sizeof(char));
    changeDoubletoIEEE(&a, ac);
    for (int i = 0; i <= 15; i++) {
        printf("%c ", ac[i]);
    }
    printf("\n");
    printf("%lf\n", changeIEEEtoDouble(ac));
}
