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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "combinationToMidi.h"

#define NB_BUTTONS 8
#define NB_NOTES 27
// Whether we want to add or subtract an octave to the note (1/0/-1)
#define OCTAVE 0

// If you want to correct false combination to possible one
// Disclaimer : play a real note, but doesn't mean
// it will sound great...

#define CORRECTION 0

/* Key combination definition.
 * In case we want to change the bindings, all we have to do is
 * change the following constants.
 *
 * Example:
 * If we want to change the binding of the C key to the first 4 buttons,
 * we would change the C constant to:
 *
 * const uint8_t C = (1<<0) | (1<<1) | (1<<2) | (1<<3);
 */

static const uint8_t CSharp = 0;
static const uint8_t C = (1<<1);
static const uint8_t B = 1;
static const uint8_t Bb = 1 | (1<<3);
static const uint8_t ASharp = 1 | (1<<1) | (1<<3);
static const uint8_t A = 1 | (1<<1);
static const uint8_t GSharp = 1 | (1<<1) | (1<<2) | (1<<3);
static const uint8_t G = 1 | (1<<1) | (1<<2);
static const uint8_t FSharp = 1 | (1<<1) | (1<<2) | (1<<5);
static const uint8_t F = 1 | (1<<1) | (1<<2) | (1<<4);
static const uint8_t E = 1 | (1<<1) | (1<<2) | (1<<4) | (1<<5);
static const uint8_t DSharp = 1 | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);
static const uint8_t D = 1 | (1<<1) | (1<<2) | (1<<4) | (1<<5) | (1<<6);
static const uint8_t CSharpPlus = CSharp | (1<<7);
static const uint8_t CPlus = C | (1<<7);
static const uint8_t BPlus = B | (1<<7);
static const uint8_t BbPlus = Bb | (1<<7);
static const uint8_t ASharpPlus = ASharp | (1<<7);
static const uint8_t APlus = A | (1<<7);
static const uint8_t GSharpPlus = GSharp | (1<<7);
static const uint8_t GPlus = G | (1<<7);
static const uint8_t FSharpPlus = FSharp | (1<<7);
static const uint8_t FPlus = F | (1<<7);
static const uint8_t EPlus = E | (1<<7);
static const uint8_t DSharpPlus = DSharp | (1<<7);
static const uint8_t DPlus = D | (1<<7);
static const uint8_t DPlusPlus = (1<<1) | (1<<2) | (1<<7);

/* 
 * A structure to hold the note information.
 * A note is defined by a key mask, a MIDI pitch and a name.
 * The key mask is a bit mask that represents the keys that are pressed.
 * The MIDI pitch is the MIDI pitch of the note according 
 * to the MIDI standard. The name is the name of the note.
 *
 * Example:
 * The C note is defined by the following:
 *       
 *       keyMask = 0b00000010
 *       midiPitch = 72
 *       name = "C"
 */

typedef struct {
    uint8_t keyMask;
    uint8_t midiPitch;
    char *name;
} note_t;


/* 
 * Macro to initialize a note
 * This macro is used to initialize a note structure. 
 * It takes 3 arguments:
 * 	- k: the key mask
 * 	- m: the MIDI pitch
 * 	- n: the name of the note
 *
 * In case the midiPitch is greater than 127, it will 
 * be truncated to 127.
 *
 * Example: To initialize the C note, we would use the following:
 *
 * NOTE_INIT(C, 72, "C")
 */

#define NOTE_INIT(k, m, n) {.keyMask=k, .midiPitch=((m)&(0xff)), .name=n}

static const note_t noteArray[NB_NOTES] = {
    NOTE_INIT(CSharp, 73+OCTAVE*12, "C#"),
    NOTE_INIT(C, 72+OCTAVE*12, "C"),
    NOTE_INIT(B, 71+OCTAVE*12, "B"),
    NOTE_INIT(Bb, 70+OCTAVE*12, "Bb"),
    NOTE_INIT(ASharp, 70+OCTAVE*12, "A#"),
    NOTE_INIT(A, 69+OCTAVE*12, "A"),
    NOTE_INIT(GSharp, 68+OCTAVE*12, "G#"),
    NOTE_INIT(G, 67+OCTAVE*12, "G"),
    NOTE_INIT(FSharp, 66+OCTAVE*12, "F#"),
    NOTE_INIT(F, 65+OCTAVE*12, "F"),
    NOTE_INIT(E, 64+OCTAVE*12, "E"),
    NOTE_INIT(DSharp, 63+OCTAVE*12, "D#"),
    NOTE_INIT(D, 62+OCTAVE*12, "D"),
    NOTE_INIT(CSharpPlus, 85+OCTAVE*12, "+C#"),
    NOTE_INIT(CPlus, 84+OCTAVE*12, "+C"),
    NOTE_INIT(BPlus, 83+OCTAVE*12, "+B"),
    NOTE_INIT(BbPlus, 82+OCTAVE*12, "+Bb"),
    NOTE_INIT(ASharpPlus, 82+OCTAVE*12, "+A#"),
    NOTE_INIT(APlus, 81+OCTAVE*12, "+A"),
    NOTE_INIT(GSharpPlus, 80+OCTAVE*12, "+G#"),
    NOTE_INIT(GPlus, 79+OCTAVE*12, "+G"),
    NOTE_INIT(FSharpPlus, 78+OCTAVE*12, "+F#"),
    NOTE_INIT(FPlus, 77+OCTAVE*12, "+F"),
    NOTE_INIT(EPlus, 76+OCTAVE*12, "+E"),
    NOTE_INIT(DSharpPlus, 75+OCTAVE*12, "+D#"),
    NOTE_INIT(DPlus, 74+OCTAVE*12, "+D"),
    NOTE_INIT(DPlusPlus, 86+OCTAVE*12, "++D")
};


uint8_t combinationToMidi(uint8_t sensorValues) {
    /*
	 * This function takes the sensor values and 
	 * returns the MIDI pitch of the note that corresponds
	 * to the sensor values.
	 *
	 * If the sensor values do not correspond to any note, 
	 * the function looks for a close key combination. 
	 * If none is found, it returns 127.
     */

	if (CORRECTION){
		for (int i = 0; i < NB_NOTES; i++) {
			if (noteArray[i].keyMask == sensorValues) {
				return noteArray[i].midiPitch;
			}
		}

		for (int i = 0; i < NB_NOTES; i++) {
			if (hammingDistance(sensorValues, noteArray[i].keyMask) == 1) {
				return noteArray[i].midiPitch;
			}
		}
		for (int i = 0; i < NB_NOTES; i++) {
			if (hammingDistance(sensorValues, noteArray[i].keyMask) == 2) {
				return noteArray[i].midiPitch;
			}
		}
	}
    return 127;
}

const char * fromSensorToPitchName(uint8_t sensorValues) {
    /*
	 * This function takes the sensor values and returns 
	 * the name of the note that corresponds to 
	 * the sensor values.
	 *
	 * If the sensor values do not correspond to any note, 
	 * the function looks for a close key combination.
	 * If none is found, it returns "not a note".
     */

	if (CORRECTION){
		for (int i = 0; i < NB_NOTES; i++) {
			if (noteArray[i].keyMask == sensorValues) {
				return noteArray[i].name;
			}
		}

		for (int i = 0; i < NB_NOTES; i++) {
			if (hammingDistance(sensorValues, noteArray[i].keyMask) == 1) {
				return noteArray[i].name;
			}
		}
		for (int i = 0; i < NB_NOTES; i++) {
			if (hammingDistance(sensorValues, noteArray[i].keyMask) == 2) {
				return noteArray[i].name;
			}
		}
	}
    return "not a note";
}


uint8_t hammingDistance(uint8_t a, uint8_t b) {
    /*
	 * This function calculates the Hamming 
	 * distance between two numbers.
	 *
	 * The Hamming distance is the number of positions 
	 * at which the corresponding bits are different.
	 *
     * Example:
     * The Hamming distance between 0b1010 and 0b1001 is 2.
     */

    uint8_t distance = 0;
    uint8_t c = a ^ b;
    while (c) {
        distance++;
        c &= c - 1;
    }
    return distance;
}
