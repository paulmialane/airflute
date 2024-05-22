#include <stdio.h>

int *is_pressed(void){
    int res[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    return (&res);
}