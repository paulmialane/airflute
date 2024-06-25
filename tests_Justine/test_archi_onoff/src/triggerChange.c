#include "triggerChange.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include "basicFunctions.h"
#include "blowCaptor.h"
#include "buttonStatus.h"


void triggerChangeThread(struct k_fifo* currentlyPlayingFifo, struct k_fifo* buttonsCombinationFifo, struct k_fifo* noteToPlayFifo){

	while(1){
		k_msleep(500);
		printk("still alive");
		int isBlowing = 0;
		isBlowing = blowingOnOff();
		printk("Est-ce qu'on souffle : %d\n", isBlowing);
		if(isBlowing){

			int oldCombination[8] = {}; // On part du principe qu'il n'y a pas d'ancienne combinaison/
			int newCombination[8] = {};
			getCombination(newCombination); // récupère l'état des capteurs

			printk("capteurs doigts récupérés\n");


			if (!k_fifo_is_empty(currentlyPlayingFifo)){ //Si on est déjà en train de jouer une note, on la récupère

				struct note_data *rx_data = k_fifo_peek_head(currentlyPlayingFifo); // attention ! on ne récupère pas les données (ce qui les enlèverait de la file), on regarde juste

				copyArray(oldCombination, rx_data->buttons);

				printk("ancienne combinaison récupérée\n");
			}

			if (!isEqual(oldCombination, newCombination)){ 
			
			/*
				On teste si la combinaison actuelle est différente de la 
				dernière (et donc s'il faut changer de note), cela marche aussi s'il n'y a pas de dernière 
				combinaison car on n'était pas en train de jouer
			*/

			    printk("on a changé de note\n");

				/*arrêter de jouer l'ancienne note*/
				if (!k_fifo_is_empty(currentlyPlayingFifo)){ 
					
					struct note_data *rx_data = k_fifo_get(currentlyPlayingFifo, K_FOREVER);
					rx_data->on = 0;
					size_t size = sizeof(struct note_data);
					char *mem_ptr = k_malloc(size);
					__ASSERT_NO_MSG(mem_ptr != 0);

					memcpy(mem_ptr, rx_data, size);

					k_fifo_put(noteToPlayFifo, mem_ptr);
					k_free(rx_data);
				}

				/*nouvelle combinaison*/

				struct note_data tx_data = { .buttons = {}, .strength = 127, .note = 0, .on = 1};

				copyArray(tx_data.buttons, newCombination);

				size_t size = sizeof(struct note_data);
				char *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, &tx_data, size);

				k_fifo_put(buttonsCombinationFifo, mem_ptr);

				printk("nouvelle combi mise dans la file\n");

			}

		}

		else {
			
			if (!k_fifo_is_empty(currentlyPlayingFifo)){

				printk("il faut arrêter de jouer\n");

				struct note_data *rx_data = k_fifo_get(currentlyPlayingFifo, K_FOREVER); 

				rx_data->on = 0;

				/*Ici on enlève directement de la file, car on arrête de jouer de toute manière...*/

				size_t size = sizeof(struct note_data);
				char *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, rx_data, size);

				k_fifo_put(noteToPlayFifo, mem_ptr);
				k_free(rx_data);
				printk("on a demandé d'arrêter de jouer l'ancienne note\n");
			}
		}
	}
}
