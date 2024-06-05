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
	int buttons[8];
	int strength;
	int note;
};

bool is_equal(combi1, combi2){
	for (int i = 0; i < 8; i++)
	{
		if (combi1[i] != combi2[i]){
			return false;
		} 
	}
	return true;
};


K_FIFO_DEFINE(buttons_combination);
K_FIFO_DEFINE(note_to_play);
K_FIFO_DEFINE(currently_playing);
K_FIFO_DEFINE(stop_playing);


///////////////////////////button_pressed main///////////////////////////////////////////////////////////


void test_buttons(void){
	while(1){

		k_msleep(500);
		if(souffle_yes_no()){

			int old_combi[8] = NULL; // On part du principe qu'il n'y a pas d'ancienne combinaison/
			int combinaison[8] = is_pressed();

			if (!k_fifo_is_empty(currently_playing)){ //S'il y en a une, on la récupère

				struct printk_data_t *rx_data = k_fifo_peep_head(currently_playing); // attention ! on ne récupère pas les données (ce qui les enlèverait de la file), on regarde juste

				old_combi = rx_data->buttons;
			}

			if (!is_equal(old_combi, combinaison)){ /*On teste si la combinaison actuelle est différente de la 
			dernière (et donc s'il faut changer de note), cela marche aussi s'il n'y a pas de dernière 
			combinaison car on n'était pas en train de jouer*/

				/*arrêter de jouer l'ancienne note*/

				struct printk_data_t *rx_data = k_fifo_get(currently_playing);

				size_t size = sizeof(struct printk_data_t);
				char *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, &rx_data, size);

				k_fifo_put(&stop_playing, mem_ptr);

				/*nouvelle combinaison*/

				struct printk_data_t tx_data = { .buttons = combinaison, .strength = NULL, .note = NULL };

				size_t size = sizeof(struct printk_data_t);
				char *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, &tx_data, size);

				k_fifo_put(&buttons_combination, mem_ptr);

			}

		}

		else {
			if (!k_fifo_is_empty(currently_playing)){

				struct printk_data_t *rx_data = k_fifo_get(currently_playing); 

				/*Ici on enlève directement de la file, car on arrête de jouer de toute manière...*/

				size_t size = sizeof(struct printk_data_t);
				char *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, &rx_data, size);

				k_fifo_put(&stop_playing, mem_ptr);
			}
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