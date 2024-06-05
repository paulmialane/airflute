#include <stdio.h>

int *is_pressed(void){
    int res[8] = NULL;

    for (int i = 0; i < 8; i++)
    {
        res[i] = 0;
    }
    
    res[1] = 1;
    res[2] = 1;
}