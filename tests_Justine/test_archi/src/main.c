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
	int *buttons;
	int strength;
	int note;
};


K_FIFO_DEFINE(buttons_combination);
K_FIFO_DEFINE(note_to_play);



///////////////////////////button_pressed main///////////////////////////////////////////////////////////


void test_buttons(void){
	while(1){
		k_msleep(1000);

		int souffle = souffle_yes_no();

		if (souffle){
			int *combi = is_pressed();
			int force = souffle_force();

			struct printk_data_t tx_data = { .buttons = combi , .strength = force , .note = NULL};

			size_t size = sizeof(struct printk_data_t);
			char *mem_ptr = k_malloc(size);
			__ASSERT_NO_MSG(mem_ptr != 0);

			memcpy(mem_ptr, &tx_data, size);

			k_fifo_put(&buttons_combination, mem_ptr);
		}
		
	}
}


///////////////////choosing the note depending on the combination of buttons pressed////////////////


void choose_note(void){
	while(1){
		struct printk_data_t *rx_data = k_fifo_get(&buttons_combination, K_FOREVER);

		int *combi = rx_data->buttons;
		int chosen_note = midi_note(combi);
		int force = rx_data->strength;

		k_free(rx_data);

		struct printk_data_t tx_data = { .buttons = NULL , .strength = force , .note = chosen_note};

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



	}
}