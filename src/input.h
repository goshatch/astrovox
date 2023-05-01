#ifndef INPUT_H
#define INPUT_H
#include <portmidi.h>

struct input {
	int midi_device_id;
	const PmDeviceInfo *selected_midi_device_info;
	PortMidiStream *midi_stream;
};

struct input input_init(void);
void input_open_midi_stream(struct input *input);
void input_select_midi_device(struct input *input);
void input_teardown_midi(void);

#endif
