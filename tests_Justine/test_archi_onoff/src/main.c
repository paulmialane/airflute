#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>

#include "basicFunctions.h"
#include "blinkLed.h"
#include "playNote.h"
#include "transformMidi.h"
#include "triggerChange.h"


/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7


K_FIFO_DEFINE(currentlyPlayingFifo);
K_FIFO_DEFINE(buttonsCombinationFifo);
K_FIFO_DEFINE(noteToPlayFifo);


void triggerChangeThreadInit(){
    triggerChangeThread(&currentlyPlayingFifo, &buttonsCombinationFifo, &noteToPlayFifo);
}

void transformMidiThreadInit(){
    transformMidiThread(&buttonsCombinationFifo, &noteToPlayFifo);
}

void playNoteThreadInit(){
    playNoteThread(&noteToPlayFifo, &currentlyPlayingFifo);
}


K_THREAD_DEFINE(blinkId, STACKSIZE, blink, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(triggerChangeId, STACKSIZE, triggerChangeThreadInit, NULL, NULL, NULL, PRIORITY, 0, 5000);
K_THREAD_DEFINE(transformMidiId, STACKSIZE, transformMidiThreadInit, NULL, NULL, NULL, PRIORITY, 0, 5000);
K_THREAD_DEFINE(playNoteId, STACKSIZE, playNoteThreadInit, NULL, NULL, NULL, PRIORITY, 0, 0);
