#include "transformMidi.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include "basicFunctions.h"
#include "combinationToMidi.h"


void transformMidiThread(struct k_fifo* buttonsCombinationFifo, struct k_fifo* noteToPlayFifo){

	while(1){
		struct note_data *rx_data = k_fifo_get(buttonsCombinationFifo, K_FOREVER);

		printk("on a récupéré une combinaison à transformer en note\n");

		int combination[8] = {};
		copyArray(combination, rx_data->buttons);
		int correspondingNote = toMidi(combination);
		int force = rx_data->strength;

		printk("note choisie = %d\n", correspondingNote);

		k_free(rx_data);

		struct note_data tx_data = { .buttons = {} , .strength = 127 , .note = correspondingNote, .on = 1};

		copyArray(tx_data.buttons, combination);

		size_t size = sizeof(struct note_data);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(noteToPlayFifo, mem_ptr);

		printk("note à jouer mise dans la file\n");
	}
}