#ifndef INPUT_H
#define INPUT_H

struct input {
	int note;
	int octave;
};

struct input init_input(void);

#endif
