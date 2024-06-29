#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>

#include "dataType.h"
#include "mainCycle.h"
#include "blowCaptor.h"
#include "buttonStatus.h"
#include "combinationToMidi.h"
#include "playSingleNote.h"


void mainCycleThread(k_fifo* noteToPlayFifo){

	midiInitialize();
	printk("------------------\nMidi initialisé\n------------------\n\n");

	struct note_data currentlyPlaying = {.buttons = -1, .note = -1, .on = 0, .strength = 0};
	/*
	Initialization of the "note we are currently playing" 
	(except we are not playing anything at the beginning, so we use impossible values)
	*/

	while(1){
		k_msleep(500);
		// We check if we are blowing
		int isBlowing;
		isBlowing = blowingOnOff();
		printk("Est-ce qu'on souffle : %d\n", isBlowing);

		if(isBlowing){
			// If we are blowing, we get the old combination of buttons, then the new
			int oldCombination = currentlyPlaying.buttons; 
			int newCombination;
			newCombination = getCombination();

			printk("Capteurs doigts récupérés\n");


			if (oldCombination != newCombination){ 
			
				/*
				If the combination changed since the last note played 
				(takes into account the case where we were not playing beforehand)
				*/

			    printk("On a changé de note\n");

				/*We need to stop playing the current note if we are playing one*/

				if (currentlyPlaying.on){ 
					// If we were playing a note, we stop playing it

					currentlyPlaying.on = 0;

					struct note_data tx_data = {.buttons = {}, .strength = force , .note = note, .on = 1};

					copyArray(tx_data.buttons, combinaison);
					
					// This SHOULD BE unnecessary
					size_t size = sizeof(struct note_data);
					struct note_data *mem_ptr = k_malloc(size);
					__ASSERT_NO_MSG(mem_ptr != 0);

					memcpy(mem_ptr, &tx_data, size);
				}

				// Now that we stopped playing the note, we can play the new one

				int newNote;
				newNote = toMidi(newCombination);

				currentlyPlaying.buttons = newCombination;
				currentlyPlaying.note = newNote;
				currentlyPlaying.strength = 127;
				currentlyPlaying.on = 1;

				sendNote(1, currentlyPlaying.note, currentlyPlaying.strength);

			}

		}

		else {

			// If we are not blowing, we stop playing the note
			if (currentlyPlaying.on){ 
					// If we were playing a note, we stop playing it

					/*modification de currentlyPlaying pour éviter des problèmes :
					on n'est rien en train de jouer, donc on indique avec on=0 et buttons=-1*/
					currentlyPlaying.on = 0;
					currentlyPlaying.buttons = -1;

					sendNote(0, currentlyPlaying.note, currentlyPlaying.strength);
				}
		}
	}
}
