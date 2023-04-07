#ifndef VOICE_H
#define VOICE_H

#include "envelope.h"
#include "input.h"
#include "oscillator.h"

struct voice {
	struct oscillator osc;
	struct envelope env;
	struct input input;
	// TODO: struct filter fiter;
};

struct voice init_voice(void);

#endif
