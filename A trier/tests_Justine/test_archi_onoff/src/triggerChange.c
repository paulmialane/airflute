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
		printk("STILL ALIVE\n");

		// We check if we are blowing
		int isBlowing;
		isBlowing = blowingOnOff();
		printk("Est-ce qu'on souffle : %d\n", isBlowing);

		if(isBlowing){
			// If we are blowing, we get the combination of buttons
			int oldCombination[8] = {}; // On part du principe qu'il n'y a pas d'ancienne combinaison/
			int newCombination[8] = {};
			getCombination(newCombination);

			printk("Capteurs doigts récupérés\n");


			if (!k_fifo_is_empty(currentlyPlayingFifo)){ 
				 // If we were playing a note, check what are we playing
				struct note_data *rx_data = k_fifo_peek_head(currentlyPlayingFifo); // attention ! on ne récupère pas les données (ce qui les enlèverait de la file), on regarde juste

				copyArray(oldCombination, rx_data->buttons);

				printk("Ancienne combinaison récupérée\n");
			}

			if (!isEqual(oldCombination, newCombination)){ 
			
			/*
				On teste si la combinaison actuelle est différente de la 
				dernière (et donc s'il faut changer de note), cela marche aussi s'il n'y a pas de dernière 
				combinaison car on n'était pas en train de jouer
			*/

			    printk("On a changé de note\n");

				/*We need to stop playing the current note*/
				if (!k_fifo_is_empty(currentlyPlayingFifo)){ 
					// If we were playing a note, we stop playing it
					struct note_data *rx_data = k_fifo_get(currentlyPlayingFifo, K_FOREVER);
					rx_data->on = 0;
					size_t size = sizeof(struct note_data);
					struct note_data *mem_ptr = k_malloc(size);
					__ASSERT_NO_MSG(mem_ptr != 0);

					memcpy(mem_ptr, rx_data, size);

					// We put the note OFF in the fifo
					k_fifo_put(noteToPlayFifo, mem_ptr);

					// As we don't need rx_data anymore, we free the memory
					k_free(rx_data);
				}

				// Now that we stopped playing the note, we can play the new one

				struct note_data tx_data = { .buttons = {}, .strength = 127, .note = 0, .on = 1};
				copyArray(tx_data.buttons, newCombination);

				// This SHOULD BE unnecessary
				size_t size = sizeof(struct note_data);
				struct note_data *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, &tx_data, size);

				k_fifo_put(buttonsCombinationFifo, mem_ptr);
				printk("Nouvelle combi mise dans la file\n");

			}

		}

		else {
			// If we are not blowing, we stop playing the note
			if (!k_fifo_is_empty(currentlyPlayingFifo)){
				// If we were playing a note, we stop playing it

				// We get the note we are currently playing
				struct note_data *rx_data = k_fifo_get(currentlyPlayingFifo, K_FOREVER); 

				rx_data->on = 0;

				/*Ici on enlève directement de la file, car on arrête de jouer de toute manière...*/

				size_t size = sizeof(struct note_data);
				char *mem_ptr = k_malloc(size);
				__ASSERT_NO_MSG(mem_ptr != 0);

				memcpy(mem_ptr, rx_data, size);

				// We put the note OFF in the fifo
				k_fifo_put(noteToPlayFifo, mem_ptr);

				// As we don't need rx_data anymore, we free the memory
				k_free(rx_data);
				printk("On a demandé d'arrêter de jouer l'ancienne note\n");
			}
		}
	}
}
