#include "voice.h"
#include "filter.h"
#include "main.h"

struct voice
init_voice(void)
{
	struct voice voice = {
		.osc = init_osc(SAWTOOTH_WAVE, 0.0f),
		.env = init_env(0.1f, 0.1f, 0.8f, 0.5f, SAMPLE_RATE),
		.filter = init_low_pass_filter(44100.0f, 1000.0f, 0.5f),
		.input = init_input()
	};

	return voice;
}
