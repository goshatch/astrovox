#ifndef INPUT_H
#define INPUT_H
#include <portmidi.h>

struct input {
	int midi_device_id;
	const PmDeviceInfo *selected_midi_device_info;
	PortMidiStream *midi_stream;
};

struct input init_input(void);
void open_midi_stream(struct input *input);
void select_midi_device(struct input *input);
void teardown_midi(void);

#endif
