#ifndef MAIN_H
#define MAIN_H

#define SAMPLE_RATE 44100.0
#define A4_FREQUENCY 440.0

// TODO: This is temporary and shoud not be hardcoded
#define MIDI_CHANNEL 0

void sigint_handler(int sig);
float note_frequency(int note_pos);

#endif
