/*
 * MIT License
 * 
 * Copyright (c) 2024  Justine OGER, Lucas MICHEL, Paul MIALANE
 * 
 * This file includes work that is based on or derived from code by Per Gantelius.
 * 
 * Original Copyright (c) 2023 Per Gantelius
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

#ifndef _BLE_MIDI_H_
#define _BLE_MIDI_H_

#include <zephyr/bluetooth/uuid.h>

/** UUID of the BLE MIDI service */
#define BLE_MIDI_SERVICE_UUID BT_UUID_128_ENCODE(0x03B80E5A, 0xEDE8, 0x4B33, 0xA751, 0x6CE34EC4C700)

/** UUID of the MIDI data I/O characteristic */
#define BLE_MIDI_CHAR_UUID    BT_UUID_128_ENCODE(0x7772E5DB, 0x3868, 0x4112, 0xA1A9, 0xF2669D106BF3)

/** Called when the BLE MIDI service becomes available/unavailable. */
typedef void (*ble_midi_available_cb_t)(uint32_t is_available);
/** Called when a BLE MIDI packet has just been sent. */
typedef void (*ble_midi_tx_done_cb_t)();
/** Called when a non-sysex message has been parsed */
typedef void (*ble_midi_message_cb_t)(uint8_t *bytes, uint8_t num_bytes, uint16_t timestamp);
/** Called when a sysex message starts */
typedef void (*ble_midi_sysex_start_cb_t)(uint16_t timestamp);
/** Called when a sysex data byte has been received */
typedef void (*ble_midi_sysex_data_cb_t)(uint8_t data_byte);
/** Called when a sysex message ends */
typedef void (*ble_midi_sysex_end_cb_t)(uint16_t timestamp);

/** Callbacks set to NULL are ignored. */
struct ble_midi_callbacks {
	ble_midi_available_cb_t available_cb;
	ble_midi_tx_done_cb_t tx_done_cb;
	ble_midi_message_cb_t midi_message_cb;
	ble_midi_sysex_start_cb_t sysex_start_cb;
	ble_midi_sysex_data_cb_t sysex_data_cb;
	ble_midi_sysex_end_cb_t sysex_end_cb;
};

/**
 * Initializes the BLE MIDI service. This should only be called once.
 */
void ble_midi_init(struct ble_midi_callbacks *callbacks);

/**
 * Sends a non-sysex MIDI message.
 * @param bytes A zero padded buffer of length 3 containing the message bytes to send.
 * @return 0 on success or a non-zero number on failure.
 */
int ble_midi_tx_msg(uint8_t *bytes);

/**
 * Start transmission of a sysex message.
 * @return 0 on success or a non-zero number on failure.
 */
int ble_midi_tx_sysex_start();

/**
 * Transmit sysex data bytes.
 * @param bytes The data bytes to send. Must have the high bit set to 0.
 * @param num_bytes The number of data bytes to send.
 * @return On success, the number of bytes written. A negative number on error.
 */
int ble_midi_tx_sysex_data(uint8_t *bytes, int num_bytes);

/**
 * End transmission of a sysex message.
 * @return 0 on success or a non-zero number on failure.
 */
int ble_midi_tx_sysex_end();

#ifdef CONFIG_BLE_MIDI_TX_MODE_MANUAL

/**
 * Send buffered MIDI messages, if any.
 */
void ble_midi_tx_buffered_msgs();
#endif

#endif
