#include "playNote.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include "basicFunctions.h"
#include "playSingleNote.h"

void playNoteThread(struct k_fifo* noteToPlayFifo, struct k_fifo* currentlyPlayingFifo){
	midiInitialize();
	printk("Midi initialisé\n------------------\n");

	while(1){
		// We get the note to play out of the fifo
		struct note_data *rx_data = k_fifo_get(noteToPlayFifo, K_FOREVER);

		printk("Note à jouer récupérée\n");

		// We extract the note, the force of the note, the combination of buttons, and whether the note is on or off
		int note = rx_data->note;
		int force = rx_data->strength;
		int combinaison[8] = {};
		copyArray(combinaison, rx_data->buttons);
		int on = rx_data->on;

		// As we don't need rx_data anymore, we free the memory
		k_free(rx_data);

		if (isConnectedOverBLE()){
			// If we are connected to a device over BLE, we play the note
			printk("Je vais jouer une note\n");
			sendNote(on, note, force);
			printk("Note jouée\n");

			// We create a new note_data struct to put in the fifo
			struct note_data tx_data = {.buttons = {}, .strength = force , .note = note, .on = 1};

			copyArray(tx_data.buttons, combinaison);
			
			// This SHOULD BE unnecessary
			size_t size = sizeof(struct note_data);
			struct note_data *mem_ptr = k_malloc(size);
			__ASSERT_NO_MSG(mem_ptr != 0);

			memcpy(mem_ptr, &tx_data, size);

			if (on){
				k_fifo_put(currentlyPlayingFifo, mem_ptr);
				printk("On a indiqué dans la file qu'on était en train de jouer\n");
			}

		}
		
		else{
			printk("On ne joue rien car on n'est pas connecté en Bluetooth...\n");
		}

	}
}