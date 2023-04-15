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

	// Initialize PortMIDI
	PmError err = Pm_Initialize();
	if (err != pmNoError) {
		fprintf(stderr, "Error initializing PortMidi: %s\n", Pm_GetErrorText(err));
		exit(1);
	}

	select_midi_device(&input);
	open_midi_stream(&input);

	return input;
}

void
open_midi_stream(struct input *input)
{
	PmError err = Pm_OpenInput(&input->midi_stream, input->midi_device_id, NULL, 512, NULL, NULL);
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

void
select_midi_device(struct input *input)
{
	PmError err = Pm_Initialize();
	if (err != pmNoError) {
		fprintf(stderr, "Error initializing PortMidi: %s\n", Pm_GetErrorText(err));
		exit(1);
	}

	int num_devices = Pm_CountDevices();
	printf("Available MIDI devices:\n");
	for (int i = 0; i < num_devices; i++) {
		const PmDeviceInfo *device_info = Pm_GetDeviceInfo(i);
		printf("%d: %s %s\n", i, device_info->interf, device_info->name);
	}

	int selected_device;
	printf("Enter the index of the MIDI device you want to use: ");
	scanf("%d", &selected_device);

	if (selected_device < 0 || selected_device >= num_devices) {
		printf("Invalid device index.\n");
		select_midi_device(input);
	} else {
		input->midi_device_id = selected_device;
		input->selected_midi_device_info = Pm_GetDeviceInfo(selected_device);
	}
}
