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
        sprintf((char *) tempprint, "%02x", temp[i]);
        strcat((char *) ret, (char *) tempprint);
    }
}

double changeIEEEtoDouble(unsigned char *IEEE) {
    double *pd, module;
    unsigned char *pc;
    pd = &module;
    pc = (unsigned char *) &module;
    for (int i = 0; i <= 15; i++) {
        if (IEEE[i]=='0'&&i%2==0){
            *pc=0x0000;
        }
    }
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
