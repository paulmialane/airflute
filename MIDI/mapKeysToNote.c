#include <stdio.h>
#include <stdbool.h>

bool compareArrays(int *arr1, int *arr2, int size);
void mapKeysToNote(int *keys, char *note);
int noteToMidiPitch(char *note);


int main() {
    int keys[8] = {1, 1, 1, 1, 0, 0, 0, 0};
    char note[3];
    mapKeysToNote(keys, note);
    int midiPitch = noteToMidiPitch(note);
    printf("Note: %s\nMIDI Pitch: %d\n", note, midiPitch);
    return 0;
}

bool compareArrays(int *arr1, int *arr2, int size) {
    for (int i = 0; i < size; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}


void mapKeysToNote(int *keys, char note[]) {
    int CSharp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int C[8] = {0, 0, 1, 0, 0, 0, 0, 0};
    int B[8] = {0, 1, 0, 0, 0, 0, 0, 0};
    int Bb[8] = {0, 1, 0, 0, 1, 0, 0, 0};
    int A[8] = {0, 1, 1, 0, 0, 0, 0, 0};
    int GSharp[8] = {0, 1, 1, 1, 1, 0, 0, 0};
    int G[8] = {0, 1, 1, 1, 0, 0, 0, 0};
    int FSharp[8] = {0, 1, 1, 1, 0, 0, 1, 0};
    int F[8] = {0, 1, 1, 1, 0, 1, 0, 0};
    int E[8] = {0, 1, 1, 1, 0, 1, 1, 0};
    int DSharp[8] = {0, 1, 1, 1, 1, 1, 1, 1};
    int D[8] = {0, 1, 1, 1, 0, 1, 1, 1};
    int CSharpPlus[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    int CPlus[8] = {1, 0, 1, 0, 0, 0, 0, 0};
    int BPlus[8] = {1, 1, 0, 0, 0, 0, 0, 0};
    int BbPlus[8] = {1, 1, 0, 0, 1, 0, 0, 0};
    int APlus[8] = {1, 1, 1, 0, 0, 0, 0, 0};
    int GSharpPlus[8] = {1, 1, 1, 1, 1, 0, 0, 0};
    int GPlus[8] = {1, 1, 1, 1, 0, 0, 0, 0};
    int FSharpPlus[8] = {1, 1, 1, 1, 0, 0, 1, 0};
    int FPlus[8] = {1, 1, 1, 1, 0, 1, 0, 0};
    int EPlus[8] = {1, 1, 1, 1, 0, 1, 1, 0};
    int DSharpPlus[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    int DPlus[8] = {1, 1, 1, 1, 0, 1, 1, 1};

    if (compareArrays(keys, CSharp, 8)){
        note[0] = 'C';
        note[1] = '#';
    }
    if (compareArrays(keys, C, 8)){
        note[0] = 'C';
    }
    if (compareArrays(keys, B, 8)){
        note[0] = 'B';
    }
    if (compareArrays(keys, Bb, 8)){
        note[0] = 'B';
        note[1] = 'b';
    }

    if (compareArrays(keys, A, 8)){
        note[0] = 'A';
    }

    if (compareArrays(keys, GSharp, 8)){
        note[0] = 'G';
        note[1] = '#';
    }

    if (compareArrays(keys, G, 8)){
        note[0] = 'G';
    }

    if (compareArrays(keys, FSharp, 8)){
        note[0] = 'F';
        note[1] = '#';
    }

    if (compareArrays(keys, F, 8)){
        note[0] = 'F';
    }

    if (compareArrays(keys, E, 8)){
        note[0] = 'E';
    }

    if (compareArrays(keys, DSharp, 8)){
        note[0] = 'D';
        note[1] = '#';
    }

    if (compareArrays(keys, D, 8)){
        note[0] = 'D';
    }

    if (compareArrays(keys, CSharpPlus, 8)){
        note[0] = '+';
        note[1] = 'C';
        note[2] = '#';
    }

    if (compareArrays(keys, CPlus, 8)){
        note[0] = '+';
        note[1] = 'C';
    }

    if (compareArrays(keys, BPlus, 8)){
        note[0] = '+';
        note[1] = 'B';
    }

    if (compareArrays(keys, BbPlus, 8)){
        note[0] = '+';
        note[1] = 'B';
        note[2] = 'b';
    }

    if (compareArrays(keys, APlus, 8)){
        note[0] = '+';
        note[1] = 'A';
    }

    if (compareArrays(keys, GSharpPlus, 8)){
        note[0] = '+';
        note[1] = 'G';
        note[2] = '#';
    }

    if (compareArrays(keys, GPlus, 8)){
        note[0] = '+';
        note[1] = 'G';
    }

    if (compareArrays(keys, FSharpPlus, 8)){
        note[0] = '+';
        note[1] = 'F';
        note[2] = '#';
    }

    if (compareArrays(keys, FPlus, 8)){
        note[0] = '+';
        note[1] = 'F';
    }

    if (compareArrays(keys, EPlus, 8)){
        note[0] = '+';
        note[1] = 'E';
    }

    if (compareArrays(keys, DSharpPlus, 8)){
        note[0] = '+';
        note[1] = 'D';
        note[2] = '#';
    }

    if (compareArrays(keys, DPlus, 8)){
        note[0] = '+';
        note[1] = 'D';
    }
}

int noteToMidiPitch(char *note) {
    if (note == "C#") {
        return 73;
    }
    else if (note == "C") {
        return 72;
    }
    else if (note == "B") {
        return 71;
    }
    else if (note == "Bb") {
        return 70;
    }
    else if (note == "A") {
        return 69;
    }
    else if (note == "G#") {
        return 68;
    }
    else if (note == "G") {
        return 67;
    }
    else if (note == "F#") {
        return 66;
    }
    else if (note == "F") {
        return 65;
    }
    else if (note == "E") {
        return 64;
    }
    else if (note == "D#") {
        return 63;
    }
    else if (note == "D") {
        return 62;
    }
    else if (note == "+C#") {
        return 85;
    }
    else if (note == "+C") {
        return 84;
    }
    else if (note == "+B") {
        return 83;
    }
    else if (note == "+Bb") {
        return 82;
    }
    else if (note == "+A") {
        return 81;
    }
    else if (note == "+G#") {
        return 80;
    }
    else if (note == "+G") {
        return 79;
    }
    else if (note == "+F#") {
        return 78;
    }
    else if (note == "+F") {
        return 77;
    }
    else if (note == "+E") {
        return 76;
    }
    else if (note == "+D#") {
        return 75;
    }
    else if (note == "+D") {
        return 74;
    }
    else {
        return -1;
    }
}
