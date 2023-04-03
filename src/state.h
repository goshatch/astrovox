#ifndef STATE_H
#define STATE_H

#include "oscillator.h"
#define BUFFER_SIZE 512

struct juno_state {
	struct oscillator osc;
	int note;
	int octave;
	int key_pressed;
	float time_index;
	float waveform[BUFFER_SIZE];
};

#endif
