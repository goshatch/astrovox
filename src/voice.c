#include "voice.h"
#include "main.h"

struct voice
init_voice(void)
{
	struct voice voice = {
		.osc = init_osc(SAWTOOTH_WAVE, 0.0f),
		.env = init_env(0.1f, 0.1f, 0.8f, 0.2f, SAMPLE_RATE),
		// TODO: .filter = init_filter(),
		.input = init_input()
	};

	return voice;
}
