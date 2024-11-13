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

#include "dataType.h"
#include "playSingleNote.h"
#include "bluetoothThread.h"

int bluetoothThread(struct k_fifo* noteToPlayFifo) {
	midiInitialize();
	printk("----------------\nMidi initialized\n----------------\n\n");

	while(1) {
		/*
		 * Whether we are connected to a device or not, we 
		 * pull the note from the fifo to avoid to flood
		 * the fifo with notes to play.
		 */ 

		struct note_data *rx_data = k_fifo_get(noteToPlayFifo, K_FOREVER);

		if (isConnectedOverBLE()) {
			/* 
			 * If we are connected to a device:
			 *   - We extract the data from the structure
			 *   - We play the note
			 */
			int note = rx_data->note;
			int force = rx_data->strength;
			bool on = rx_data->on;

			sendNote(on, note, force);
		} else {

			// If we are not connected to a device
			printk("Waiting for a connection\n");
		}

		// We free the memory
		k_free(rx_data);
		k_msleep(10);
	}
}
