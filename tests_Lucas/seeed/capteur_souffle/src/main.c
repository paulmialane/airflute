#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>
#include "capteur_souffle.h"

int main(){
    while(1){
        data_out = souffle_force(void);

        printk("%i\n", data_out);

        k_msleep(100);
    }
}