#ifndef STATE_H
#define STATE_H

#include "envelope.h"
#include "input.h"
#include "oscillator.h"

#define BUFFER_SIZE 512

struct juno_state {
	struct oscillator osc;
	struct envelope env;
	struct input input;
	int note;
	int octave;
	int key_pressed;
	float time_index;
	float waveform[BUFFER_SIZE];
};

#endif
