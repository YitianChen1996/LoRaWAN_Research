#include <iostream>
#include <string.h>

void changeDoubletoIEEE(double *dp, char *&ret) {
    char *cp;
    cp = (char *) dp;
    for (int i = 0; i <= 7; i++) {
        strcpy(ret + i, cp + i);
    }
}

double changeIEEEtoDouble(char *IEEE) {
    double *pd;
    pd = (double *) IEEE;
    return *pd;
}

int main() {
    double a = -75.1235432;
    char *ac;
    ac = (char *) malloc(8 * sizeof(char));
    memset(ac, 0, 8 * sizeof(char));
    changeDoubletoIEEE(&a, ac);
    for (int i = 0; i <= 7; i++) {
        printf("%02x ", ac[i]);
    }
    printf("\n");
    printf("%lf\n", changeIEEEtoDouble(ac));
}
