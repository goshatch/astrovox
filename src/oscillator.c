#include <math.h>
#include "oscillator.h"

osc_wave_gen generators[] = {osc_gen_sine_wave, osc_gen_sawtooth_wave, osc_gen_square_wave, osc_gen_pulse_wave};

// TODO: These formulas need to be confirmed!
// Sine wave: y(t) = A * sin(2 * PI * f * t)
float
osc_gen_sine_wave(float phase)
{
	return sinf(2.0f * M_PI * phase);
}

// Sawtooth wave: y(t) = A * (2 * (f * t - floor(0.5 + f * t)))
float
osc_gen_sawtooth_wave(float phase)
{
	return fmodf(phase, 1.0f) * 2.0f - 1.0f;
}

// Square wave: y(t) = A * sign(sin(2 * PI * f * t))
float
osc_gen_square_wave(float phase)
{
	return sinf(2.0 * M_PI * phase) >= 0 ? 1.0f : -1.0f;
}

float
osc_gen_pulse_wave(float phase)
{
	float pulse_width = 0.2;
	return (phase < pulse_width) ? 1.0f : -1.0f;
}

char *
osc_wave_name(enum osc_wave_types type)
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
osc_init(enum osc_wave_types type, float frequency)
{
	struct oscillator osc = {
		.frequency = frequency, .type = type, .generator = generators[type]};
	return osc;
}

void
osc_prev_wave_gen(struct oscillator *osc)
{
	osc->type = (osc->type - 1 + GEN_COUNT) % GEN_COUNT;
	osc->generator = generators[osc->type];
}

void
osc_next_wave_gen(struct oscillator *osc)
{
	osc->type = (osc->type + 1) % GEN_COUNT;
	osc->generator = generators[osc->type];
}
