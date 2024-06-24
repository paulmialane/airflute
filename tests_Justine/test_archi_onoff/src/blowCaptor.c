#include "blowCaptor.h"

int n = 0;

int blowingOnOff(void){
    n = n + 1;
    if (n%2 == 1){
        return 1;
    }
    else{
        return 0;
    }
}

int blowingStrength(void){
    return 127;
}