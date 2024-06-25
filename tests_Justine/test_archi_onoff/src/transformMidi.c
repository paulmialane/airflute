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
		// We get the combination of buttons to transform into a note
		struct note_data *rx_data = k_fifo_get(buttonsCombinationFifo, K_FOREVER);

		printk("On a récupéré une combinaison à transformer en note\n");

		// We extract the combination of buttons
		int combination[8] = {};
		copyArray(combination, rx_data->buttons);
		// We extract the force of the note
		int force = rx_data->strength;

		// We transform the combination of buttons into a note
		int correspondingNote = toMidi(combination);

		printk("Note choisie = %d\n", correspondingNote);

		// As we don't need rx_data anymore, we free the memory
		k_free(rx_data);

		// We create a new note_data struct to put in the fifo
		struct note_data tx_data = { .buttons = {} , .strength = force, .note = correspondingNote, .on = 1};
		copyArray(tx_data.buttons, combination);

		// This SHOULD BE unnecessary
		size_t size = sizeof(struct note_data);
		struct note_data *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(noteToPlayFifo, mem_ptr);

		
		printk("Note à jouer mise dans la file\n");
	}
}