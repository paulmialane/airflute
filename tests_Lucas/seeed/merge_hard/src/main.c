#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>
#include "capteur_souffle.h"
#include "button_pressed.h"


int main(){
    int reference = init_capteur();
    
    int data_out=0;
    while(1){

        int val_gpio_1 = getKeyMask(); 

        printk("\r");
        for (int i=0; i<8; i++){
            printk("%d", (val_gpio_1>>i&1));
        }

        data_out = souffle_force(reference);

        printk("     %i   ", data_out);

        k_msleep(50);
    }
}