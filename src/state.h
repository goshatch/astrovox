#ifndef STATE_H
#define STATE_H

#include <ev.h>
#include "constants.h"
#include "input.h"
#include "ui.h"
#include "voice.h"

#define NUM_VOICES 1

struct state {
	struct input input;
	struct ui ui;
	struct voice voices[NUM_VOICES];
	float wave_time_index;
	float wave_time_index_chorus;
	float vis_time_index;
	float vis_waveform[BUFFER_SIZE];
};
#endif
