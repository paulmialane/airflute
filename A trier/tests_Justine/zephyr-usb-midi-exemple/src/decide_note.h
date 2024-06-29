#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/gpio.h>
#include "usb_midi/usb_midi.h"
#include "decide_note.c"


void play_note(struct k_work *item);
int next_note_number(void);
int note_number;
