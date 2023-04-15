#ifndef STATE_H
#define STATE_H

#include "voice.h"
#include "input.h"

#define BUFFER_SIZE 512
#define NUM_VOICES 1

struct state {
	struct input input;
	struct voice voices[NUM_VOICES];
	float time_index;
	float vis_time_index;
	float vis_waveform[BUFFER_SIZE];
};

#endif
