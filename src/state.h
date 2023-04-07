#ifndef STATE_H
#define STATE_H

#include "voice.h"

#define BUFFER_SIZE 512
#define NUM_VOICES 1

struct state {
	struct voice voices[NUM_VOICES];
	float time_index;
	float vis_time_index;
	float vis_waveform[BUFFER_SIZE];
};

#endif
