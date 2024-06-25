#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <math.h>
#include "capteur_souffle.h"
#include "button_pressed.h"
#include "mapKeysToNote.h"

int main(){
    int reference = init_capteur();
    
    int data_out=0;
    while(1){

        int sensorValues = getKeyMask(); 

        printk("\r");
        
        for (int i=0; i<8; i++){
                printk("%d", (sensorValues>>i&1));
        }

        data_out = souffle_force(reference);

        printk("     %i   ", data_out);

        uint8_t midiPitch = fromSensorToMidiPitch(sensorValues);
        if (midiPitch != 127) {
        printf("  The MIDI pitch of the note is: %d                   ", midiPitch);
        } else {
        printf("  The sensor values do not correspond to any note    ");
        }

        k_msleep(50);
    }
}