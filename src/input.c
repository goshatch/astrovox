#include "input.h"

struct input
init_input(void)
{
	struct input input = {
		.note = -1,
		.octave = 3,
	};

	return input;
}
