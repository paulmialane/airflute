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

#ifndef DATATYPE_H
#define DATATYPE_H

#include <stdbool.h>
#include <stdint.h>

/*
 * This struct represent a note in the midi format.
 * It will be stored on the fifo.
 *
 * In the MIDI Protocol :
 *   - A note is  0, ..., 127
 *   - A strength (actually called velocity) is 0, ..., 127
 * 
 * On our flute a there are 8 buttons, and a combination
 * is represented with a uint8_t.
 */

struct note_data {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	uint8_t buttons; 
	uint8_t strength;
	uint8_t note;
	bool on;
};

#endif
