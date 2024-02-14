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


#include "ble_midi_context.h"

void ble_midi_context_init(struct ble_midi_context* context) {
    context->user_callbacks.available_cb = NULL;
    context->user_callbacks.midi_message_cb = NULL;
    context->user_callbacks.sysex_data_cb = NULL;
    context->user_callbacks.sysex_end_cb = NULL;
    context->user_callbacks.sysex_start_cb = NULL;
    context->user_callbacks.tx_done_cb = NULL;
    
    ble_midi_context_reset(context, 0, 0);
}

void ble_midi_context_reset(struct ble_midi_context* context, int tx_running_status, int tx_note_off_as_note_on) {
    atomic_set(&context->pending_midi_msg_work_count, 0);
    ble_midi_writer_init(&context->sysex_tx_writer, tx_running_status, tx_note_off_as_note_on);
	ble_midi_writer_init(&context->tx_writer, tx_running_status, tx_note_off_as_note_on);
}