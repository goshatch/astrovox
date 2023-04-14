#include <portmidi.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

struct input
init_input(void)
{
	struct input input = {
		.midi_device_id = -1,
	};
	init_midi(&input);

	return input;
}

void
init_midi(struct input *input)
{
	// Initialize PortMIDI
	PmError err = Pm_Initialize();
	if (err != pmNoError) {
		fprintf(stderr, "Error initializing PortMidi: %s\n", Pm_GetErrorText(err));
		exit(1);
	}
	err = Pm_OpenInput(&input->midi_stream, input->midi_device_id, NULL, 512, NULL, NULL);
	if (err != pmNoError) {
		fprintf(stderr, "Error opening PortMidi input stream: %s\n", Pm_GetErrorText(err));
		exit(1);
	}
}

void
teardown_midi(void)
{
	PmError err = Pm_Terminate();
	if (err != pmNoError) {
		fprintf(stderr, "PortMidi error: %s\n", Pm_GetErrorText(err));
	}
}
