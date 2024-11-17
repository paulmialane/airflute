/*
 * MIT License
 * 
 * Copyright (c) 2024 Justine OGER, Lucas MICHEL, Paul MIALANE
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
#include <stdbool.h>
#include <stdint.h>

#include "dataType.h"
#include "mainCycle.h"
#include "blowSensor.h"
#include "buttonStatus.h"
#include "combinationToMidi.h"
#include "playSingleNote.h"


void mainCycleThread(){

	/*
	 * This thread handles the logic of the flute
	 */

	int standard = sensorInit();

	midiInitialize();
	printk("----------------\nMidi initialized\n----------------\n\n");

	/*
	 * Initialization of the "note we are currently playing" 
	 * (except we are not playing anything at the beginning, so
	 * we use impossible values)
	 */

	struct note_data currentlyPlaying = {.buttons = 0, .note = 255, .on = 0, .strength = 255};

	while(1) {
		// We wait 50ms before looking for a change.
		k_msleep(50);

		bool isBlowing = blowingOnOff(standard);

		if(isBlowing && currentlyPlaying.on) {
			/*
			 * If we are blowing, we get the old combination
			 * of buttons, then the new and the old strength
			 */

			uint8_t oldCombination = currentlyPlaying.buttons; 
			uint8_t newCombination = getCombination();
			uint8_t oldStrength = currentlyPlaying.strength;
			uint8_t newStrength = strengthCategory(blowingStrength(standard));


			if (oldCombination != newCombination) { 
				/*
				 * If the combination changed since the last
				 * note played
				 */

				// We stop playing the current note if we are playing
				sendNote(false, currentlyPlaying.note, currentlyPlaying.strength);

				// Now that we stopped the note, we play the new one
				uint8_t newNote = combinationToMidi(newCombination);
				currentlyPlaying.buttons = newCombination;
				currentlyPlaying.note = newNote;
				currentlyPlaying.strength = newStrength;

				sendNote(currentlyPlaying.on, currentlyPlaying.note, currentlyPlaying.strength);

			}

			else {
				/*
				 * if the combination didn't change, we have
				 * to check if we started blowing with a 
				 * different strength (so if we have to change
				 * the volume)
				 */

				if (newStrength != oldStrength) {
					/*
					 * For now, we are trying to
					 * just send the same note with a different volume
					 * (without sending a note off first)
					 */
					currentlyPlaying.strength = newStrength;

					sendNote(false, currentlyPlaying.note, currentlyPlaying.strength);

					sendNote(currentlyPlaying.on, currentlyPlaying.note, currentlyPlaying.strength);
				}
			}

		} else if(isBlowing && (!currentlyPlaying.on)) {
			
			/*
			 * If we are blowing but were not already playing a note
			 */

			uint8_t newCombination = getCombination();
			uint8_t newStrength = strengthCategory(blowingStrength(standard));
			uint8_t	newNote = combinationToMidi(newCombination);

			currentlyPlaying.buttons = newCombination;
			currentlyPlaying.note = newNote;
			currentlyPlaying.on = true;
			currentlyPlaying.strength = newStrength;

			sendNote(true, currentlyPlaying.note, currentlyPlaying.strength);

		} else {

			// If we are not blowing, we stop playing the note
			if (currentlyPlaying.on) { 

				sendNote(false, currentlyPlaying.note,
						currentlyPlaying.strength);

			}

			currentlyPlaying.on = false;
		}
	}
}
