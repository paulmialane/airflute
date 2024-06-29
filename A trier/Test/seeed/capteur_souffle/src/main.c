#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>
#include "capteur_souffle.h"

int main(){
    int reference = init_capteur();
    
    int data_out=0;
    while(1){
        data_out = souffle_force(reference);

        printk("%i\n", data_out);

        k_msleep(50);
    }
}