#include <stdbool.h>

struct note_data {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	int buttons;
	int strength;
	int note;
	bool on;
};


