#ifndef STATE_H
#define STATE_H

#include <ev.h>
#include "input.h"
#include "ui.h"
#include "voice.h"

#define BUFFER_SIZE 512
#define NUM_VOICES 1

struct state {
	struct input input;
	struct ui ui;
	struct voice voices[NUM_VOICES];
	float time_index;
	float vis_time_index;
	float vis_waveform[BUFFER_SIZE];
};

struct stateful_watcher {
	struct state *state;
	union {
		ev_timer timer;
		ev_io io;
		ev_check check;
	} watcher;
};

#endif
