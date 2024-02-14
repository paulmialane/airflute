/*
 * MIT License
 * 
 * Copyright (c) 2024 Paul MIALANE, Lucas MICHEL, Justine OGER
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

#ifndef _BLE_MIDI_CONTEXT_H_
#define _BLE_MIDI_CONTEXT_H_

#include <zephyr/kernel.h>
#include <ble_midi/ble_midi.h>
#include "ble_midi_packet.h"

struct ble_midi_context {
    atomic_t pending_midi_msg_work_count;
    int is_initialized;
    struct ble_midi_writer_t tx_writer;
    struct ble_midi_writer_t sysex_tx_writer;
    struct ble_midi_callbacks user_callbacks;
};

void ble_midi_context_init(struct ble_midi_context* context);

void ble_midi_context_reset(struct ble_midi_context* context, int tx_running_status, int tx_note_off_as_note_on);

#endif // _BLE_MIDI_CONTEXT_H_