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

#ifndef _BLE_MIDI_RADIO_NOTIFICATIONS_H_
#define _BLE_MIDI_RADIO_NOTIFICATIONS_H_

typedef void (*radio_notification_cb_t)();

/**
 * Configures an interrupt that is triggered just before each BLE connection event.
 * Used to trigger transmission of BLE MIDI data accumulated between connection events.
 */
int radio_notifications_init(radio_notification_cb_t callback);

void radio_notifications_enable();
void radio_notifications_disable();

#endif // _BLE_MIDI_RADIO_NOTIFICATIONS_H_