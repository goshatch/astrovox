#include "voice.h"
#include "filter.h"
#include "main.h"
#include "oscillator.h"

struct voice
voice_init(void)
{
	struct voice voice = {
		.osc = osc_init(SAWTOOTH_WAVE, 0.0f),
		.chorus_osc = osc_init(PULSE_WAVE, 0.0f),
		.env = env_init(0.1f, 0.1f, 0.8f, 0.5f, SAMPLE_RATE),
		.filter = low_pass_filter_init(44100.0f, 1000.0f, 0.5f),
		.note = { .value = -1, .octave = 3 },
		.chorus = FALSE
	};

	return voice;
}
