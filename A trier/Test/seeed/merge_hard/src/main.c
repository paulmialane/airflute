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

        //clear terminal
        printk("\033[2J\033[H"); 

        for (int i=0; i<8; i++){
                printk("%d", (sensorValues>>i&1));
        }

        data_out = souffle_yes_no(reference);

        printk("     %i   ", data_out);

        printf("  The MIDI pitch of the note is %s (%d) ",
             fromSensorToPitchName(sensorValues) ,fromSensorToMidiPitch(sensorValues));

        k_msleep(50);
    }
}