#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#define GEN_COUNT 4
enum wave_types {
	SINE_WAVE,
	SAWTOOTH_WAVE,
	SQUARE_WAVE,
	PULSE_WAVE
};

typedef float (*wave_gen)(float);
// typedef float (*wave_gen)(float*, float);

struct oscillator {
	wave_gen generator;
	enum wave_types type;
	float frequency;
};

struct oscillator init_osc(enum wave_types type, float frequency);
float sine_wave_gen(float phase);
float sawtooth_wave_gen(float phase);
float square_wave_gen(float phase);
float pulse_wave_gen(float phase);

char *wave_name(enum wave_types type);

void prev_wave_gen(struct oscillator *osc);
void next_wave_gen(struct oscillator *osc);
#endif
