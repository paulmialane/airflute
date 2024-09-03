/*
 * MIT License
 * 
 * Copyright (c) 2024 Paul MIALANE, Lucas MICHEL, Justine OGER
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>

#include "dataType.h"
#include "mainCycle.h"
#include "blowSensor.h"
#include "buttonStatus.h"
#include "combinationToMidi.h"
#include "playSingleNote.h"


void mainCycleThread(){

	int reference;
	reference = sensorInit();

	midiInitialize();
	printk("------------------\nMidi initialisé\n------------------\n\n");

	struct note_data currentlyPlaying = {.buttons = -1, .note = -1, .on = 0, .strength = -1};
	/*
	Initialization of the "note we are currently playing" 
	(except we are not playing anything at the beginning, so we use impossible values)
	*/

	while(1){
		k_msleep(50);
		// We check if we are blowing
		int isBlowing;
		isBlowing = blowingOnOff(reference);

		if(isBlowing){
			/*If we are blowing, we get the old combination of buttons, then the new
			and the old strength, then the new*/
			int oldCombination = currentlyPlaying.buttons; 
			uint8_t newCombination;
			newCombination = getCombination();

			int oldStrength = currentlyPlaying.strength;
			int newStrength;
			newStrength= strengthCategory(blowingStrength(reference));


			if (oldCombination != newCombination){ 
			
				/*
				If the combination changed since the last note played 
				(takes into account the case where we were not playing beforehand)
				*/

				/*We need to stop playing the current note if we are playing one*/

				if (currentlyPlaying.on){ 
					// If we were playing a note, we stop playing it

					currentlyPlaying.on = 0;

					sendNote(0, currentlyPlaying.note, currentlyPlaying.strength);

					currentlyPlaying.buttons = -1;
					currentlyPlaying.note = -1;
					currentlyPlaying.strength = -1;
				}

				// Now that we stopped playing the note, we can play the new one

				uint8_t newNote;
				newNote = combinationToMidi(newCombination);

				currentlyPlaying.buttons = newCombination;
				currentlyPlaying.note = newNote;
				currentlyPlaying.on = true;
				currentlyPlaying.strength = newStrength;

				sendNote(true, currentlyPlaying.note, currentlyPlaying.strength);

			}

			else {
				/*if the combination didn't change, we have to check if we started blowing with a 
				different strength (so if we have to change the volume)*/

				if (newStrength != oldStrength){
					/*for now, we are trying to just send the same note with a different volume
					(without sending a note off first)*/
					currentlyPlaying.strength = newStrength;

					sendNote(false, currentlyPlaying.note, currentlyPlaying.strength);

					sendNote(true, currentlyPlaying.note, currentlyPlaying.strength);
				}

				/*if the combination of sensors stayed the same and the overall strength stayed the same,
				we just don't change anything*/
			}

		}

		else {

			// If we are not blowing, we stop playing the note
			if (currentlyPlaying.on){ 
					// If we were playing a note, we stop playing it

					/*modification de currentlyPlaying pour éviter des problèmes :
					on n'est rien en train de jouer, donc on indique avec on=0 et buttons=-1*/
					currentlyPlaying.on = false;
					currentlyPlaying.buttons = -1;

					sendNote(false, currentlyPlaying.note, currentlyPlaying.strength);

					currentlyPlaying.note = -1;
					currentlyPlaying.strength = -1;
				}
		}
	}
}
