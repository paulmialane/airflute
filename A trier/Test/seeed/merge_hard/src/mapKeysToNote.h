#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mapKeysToNote.c"

uint8_t fromSensorToMidiPitch(uint8_t sensorValues);

int hammingDistance(uint8_t a, uint8_t b);