#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "button_pressed.h"


int main(void) { 

    while (1){
            int val_gpio_1 = getKeyMask(); // ou gpio_pin_get(...) si on veut juste une des pins

            printk("\r");
            for (int i=0; i<8; i++){
                printk("%d", (val_gpio_1>>i&1));

            }

            k_msleep(100);
    }
}
