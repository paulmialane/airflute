#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>


#include "blinkLed.h"
#include "mainCycle.h"

#define STACKSIZE 1024
#define PRIORITY 4

void mainCycleInit(){
    // Here we initialize the main thread

    mainCycleThread();
}
 


// A thread to blink the led as it is usefull to know if the board is still alive
K_THREAD_DEFINE(blinkId, STACKSIZE, blink, NULL, NULL, NULL, PRIORITY, 0, 0);

K_THREAD_DEFINE(mainId, STACKSIZE, mainCycleInit, NULL, NULL, NULL, PRIORITY, 0, 0);