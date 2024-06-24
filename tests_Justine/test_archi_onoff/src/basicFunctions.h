#include <stdbool.h>

struct note_data {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	int buttons[8];
	int strength;
	int note;
	bool on;
};

void copyArray(int[8], int[8]);

bool isEqual(int[8], int[8]);