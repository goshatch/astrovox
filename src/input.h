#ifndef INPUT_H
#define INPUT_H
#include <portmidi.h>

struct input {
	int midi_device_id;
	PortMidiStream *midi_stream;
};

struct input init_input(void);
void init_midi(struct input *input);
void teardown_midi(void);

#endif
