#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>
#include "button_pressed.h"
#include "decide_note.h"
#include "capteur_souffle.h"
#include "joue_note.h"

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7


struct printk_data_t {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	int buttons;
	int strength;
	int note;
};


K_FIFO_DEFINE(buttons_combination);
K_FIFO_DEFINE(note_to_play);
K_FIFO_DEFINE(currently_playing);
K_FIFO_DEFINE(stop_playing);


///////////////////////////button_pressed main///////////////////////////////////////////////////////////


void test_buttons(void){
	while(1){

		k_msleep(100);



		if(souffle)

		}
		
	}
}


///////////////////choosing the note depending on the combination of buttons pressed////////////////


void choose_note(void){
	while(1){
		struct printk_data_t *rx_data = k_fifo_get(&buttons_combination, K_FOREVER);

		int combi = rx_data->buttons;
		int chosen_note = midi_note(combi);
		int force = rx_data->strength;

		k_free(rx_data);

		struct printk_data_t tx_data = { .buttons = combi , .strength = force , .note = chosen_note};

		size_t size = sizeof(struct printk_data_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(&note_to_play, mem_ptr);
	}
}


//////////////////////////////////////playing the note////////////////////////////////////////////////


void play_note(void){
	while(1){
		struct printk_data_t *rx_data = k_fifo_get(&note_to_play, K_FOREVER);

		int note = rx_data->note;
		int force = rx_data->strength;
		int combi = rx_data->buttons;

		k_free(rx_data);

		joue(true, note, force);

		struct printk_data_t tx_data = { .buttons = combi , .strength = force , .note = note};

		size_t size = sizeof(struct printk_data_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(&currently_playing, mem_ptr);

	}
}

void stop_playing_note(void){
	while(1){
		struct printk_data_t *rx_data = k_fifo_get(&stop_playing, K_FOREVER);

		int note = rx_data->note;
		int force = rx_data->strength;
		int combi = rx_data->buttons;

		k_free(rx_data);

		joue(false, note, force);

	}
}