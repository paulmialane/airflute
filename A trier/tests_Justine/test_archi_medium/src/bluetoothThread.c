#include "bluetoothThread.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include "dataType.h"

#include "playSingleNote.h"

int bluetoothThread(struct k_fifo* noteToPlayFifo){
	midiInitialize();
	printk("------------------\nMidi initialisé\n------------------\n\n");

	while(1){
		/*
		Whether we are connected to a device or not, we pull the note from the fifo
		to avoid to flood the fifo with notes to play.
		*/ 

		struct note_data *rx_data = k_fifo_get(noteToPlayFifo, K_FOREVER);

		if (isConnectedOverBLE())
		{
			/* 
			If we are connected to a device:
				- We extract the data from the structure
				- We play the note
			*/
			int note = rx_data->note;
			int force = rx_data->strength;
			bool on = rx_data->on;

			sendNote(on, note, force);
		}
		else
		{
			/* If we are not connected to a device */
			printk("Waiting for a connection\n");
		}

		// We free the memory
		k_free(rx_data);
		k_msleep(10);
	}
}