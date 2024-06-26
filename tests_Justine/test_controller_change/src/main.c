#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>


#include "blinkLed.h"
#include "playSingleNote.h"

#define STACKSIZE 1024
#define PRIORITY 4



void main(void){
    midiInitialize();

    while(1){
        k_msleep(1000);
        sendNote(true, 60, 50);
        k_msleep(500);
        printk("sent note");
        for (int i = 0; i < 25; i++)
        {
            k_msleep(100);
            changeVolume(100+i);
        }
        k_msleep(500);
        sendNote(false, 60, 0);
        printk("stopped sending note");
    }
    
}
 
