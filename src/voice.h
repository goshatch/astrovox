#ifndef VOICE_H
#define VOICE_H

#include "envelope.h"
#include "filter.h"
#include "input.h"
#include "oscillator.h"

struct note {
	int value;
	int octave;
};

struct voice {
	struct oscillator osc;
	struct oscillator chorus_osc;
	struct envelope env;
	struct note note;
	struct low_pass_filter filter;
	int chorus;
};

struct voice init_voice(void);

#endif
