#include "buttonStatus.h"
#include <stdio.h>

void getCombination(int *tab_vide){

    for (int i = 0; i < 8; i++)
    {
        tab_vide[i] = 0;
    }
    
    tab_vide[1] = 1;
    tab_vide[2] = 1;
}