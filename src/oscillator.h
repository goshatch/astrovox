#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#define GEN_COUNT 4
enum osc_wave_types {
	SINE_WAVE,
	SAWTOOTH_WAVE,
	SQUARE_WAVE,
	PULSE_WAVE
};

typedef float (*osc_wave_gen)(float);
// typedef float (*wave_gen)(float*, float);

struct oscillator {
	osc_wave_gen generator;
	enum osc_wave_types type;
	float frequency;
};

struct oscillator osc_init(enum osc_wave_types type, float frequency);
float osc_gen_sine_wave(float phase);
float osc_gen_sawtooth_wave(float phase);
float osc_gen_square_wave(float phase);
float osc_gen_pulse_wave(float phase);

char *osc_wave_name(enum osc_wave_types type);

void osc_prev_wave_gen(struct oscillator *osc);
void osc_next_wave_gen(struct oscillator *osc);
#endif
