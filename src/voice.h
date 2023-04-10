#ifndef VOICE_H
#define VOICE_H

#include "envelope.h"
#include "filter.h"
#include "input.h"
#include "oscillator.h"

struct voice {
	struct oscillator osc;
	struct envelope env;
	struct input input;
	struct low_pass_filter filter;
};

struct voice init_voice(void);

#endif
