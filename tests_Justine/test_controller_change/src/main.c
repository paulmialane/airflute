#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>


#include "blinkLed.h"
#include "playSingleNote.h"

#define STACKSIZE 1024
#define PRIORITY 4

void mainCycleInit(){
    midiInitialize();

    while(1){
        k_msleep(1000);
        sendNote(1, 60, 1);
        k_msleep(1000);
        changeNote(127);
        k_msleep(1000);
        changeNote(5);
        k_msleep(1000);
        sendNote(0, 60, 0);
    }
    
}
 


// A thread to blink the led as it is usefull to know if the board is still alive
K_THREAD_DEFINE(blinkId, STACKSIZE, blink, NULL, NULL, NULL, PRIORITY, 0, 0);

K_THREAD_DEFINE(mainId, STACKSIZE, mainCycleInit, NULL, NULL, NULL, PRIORITY, 0, 0);