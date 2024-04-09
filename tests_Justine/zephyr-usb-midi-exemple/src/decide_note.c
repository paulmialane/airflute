#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/gpio.h>
#include "usb_midi/usb_midi.h"
#include <stdbool.h> 

#define LED_FLASH_DURATION_MS 60
#define SYSEX_TX_MESSAGE_SIZE 2000
#define SYSEX_TX_CABLE_NUM    0
#define TX_INTERVAL_MS	      500
#define TX_NOTE_NUMBER	      69
#define TX_NOTE_VELOCITY      0x7f

struct k_work button_press_work;
struct k_work event_tx_work;
struct k_work_delayable rx_led_off_work;
struct k_work_delayable tx_led_off_work;

void play_note(struct k_work *item);
int next_note_number(void);
int note_number = 69;

int received[10] = {true, true, true, true, true, true, true, true, true, true};



/************************ App state ************************/
struct sample_app_state_t {
	int usb_midi_is_available;
	int sysex_rx_data_byte_count;
	int sysex_tx_data_byte_count;
	int sysex_tx_in_progress;
	int tx_note_off;
};

static struct sample_app_state_t sample_app_state = {.usb_midi_is_available = 0,
						     .sysex_tx_data_byte_count = 0,
						     .sysex_tx_in_progress = 0,
						     .sysex_rx_data_byte_count = 0,
						     .tx_note_off = 0};

/***************************** Play different notes *******************************/

void play_note(struct k_work *item)
{
    if (sample_app_state.usb_midi_is_available && !sample_app_state.sysex_tx_in_progress) {
	    	uint8_t msg[3] = {sample_app_state.tx_note_off ? 0x90 : 0x80, sample_app_state.tx_note_off ? 
                next_note_number() : note_number, TX_NOTE_VELOCITY};
		    usb_midi_tx(0, msg);
		    sample_app_state.tx_note_off = !sample_app_state.tx_note_off;
	}
}

int next_note_number(void)
{
    note_number = note_number + 4;
    return(note_number);
}