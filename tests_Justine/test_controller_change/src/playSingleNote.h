#include <stdbool.h>

void sendNote(bool on_off, int note, int force);
void midiInitialize(void);
int isConnectedOverBLE(void);
void changeNote(int);