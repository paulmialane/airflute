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
	printk("midi initialisé\n");

	while(1){
		struct note_data *rx_data = k_fifo_get(noteToPlayFifo, K_FOREVER);

		printk("note à jouer récupérée\n");

		int note = rx_data->note;
		int force = rx_data->strength;
		int combi[8] = {};
		copyArray(combi, rx_data->buttons);
		int on = rx_data->on;

		k_free(rx_data);


		if (isAvailable()){
			printk("joue une note\n");
			sendNote(on, note, force);

			struct note_data tx_data = {.buttons = {}, .strength = force , .note = note, .on = 1};

			copyArray(tx_data.buttons, combi);

			size_t size = sizeof(struct note_data);
			char *mem_ptr = k_malloc(size);
			__ASSERT_NO_MSG(mem_ptr != 0);

			memcpy(mem_ptr, &tx_data, size);

			k_fifo_put(currentlyPlayingFifo, mem_ptr);

			printk("on a indiqué dans la file qu'on était en train de jouer\n");
		}
		
		else{
			printk("on ne joue rien car on n'est pas connecté en Bluetooth...\n");
		}

	}
}