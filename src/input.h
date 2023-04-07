#ifndef INPUT_H
#define INPUT_H

struct input {
	int note;
	int octave;
	int key_pressed;
};

struct input init_input(void);

#endif
