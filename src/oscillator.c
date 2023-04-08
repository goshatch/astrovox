#include <math.h>
#include "oscillator.h"

wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen, pulse_wave_gen};

// TODO: These formulas need to be confirmed!
// Sine wave: y(t) = A * sin(2 * PI * f * t)
float
sine_wave_gen(float phase)
{
	return sinf(2.0f * M_PI * phase);
}

// Sawtooth wave: y(t) = A * (2 * (f * t - floor(0.5 + f * t)))
float
sawtooth_wave_gen(float phase)
{
	return fmodf(phase, 1.0f) * 2.0f - 1.0f;
}

// Square wave: y(t) = A * sign(sin(2 * PI * f * t))
float
square_wave_gen(float phase)
{
	return sinf(2.0 * M_PI * phase) >= 0 ? 1.0f : -1.0f;
}

float
pulse_wave_gen(float phase)
{
	float pulse_width = 0.2;
	return (phase < pulse_width) ? 1.0f : -1.0f;
}

char *
wave_name(enum wave_types type)
{
	switch (type) {
	case SINE_WAVE:
		return "SINE";
	case SAWTOOTH_WAVE:
		return "SAW ";
	case SQUARE_WAVE:
		return "SQRE";
	case PULSE_WAVE:
		return "PULS";
	default:
		return "Unknown";
	}
}

struct oscillator
init_osc(enum wave_types type, float frequency)
{
	struct oscillator osc = {
		.frequency = frequency, .type = type, .generator = generators[type]};
	return osc;
}

void
prev_wave_gen(struct oscillator *osc)
{
	osc->type = (osc->type - 1 + GEN_COUNT) % GEN_COUNT;
	osc->generator = generators[osc->type];
}

void
next_wave_gen(struct oscillator *osc)
{
	osc->type = (osc->type + 1) % GEN_COUNT;
	osc->generator = generators[osc->type];
}
