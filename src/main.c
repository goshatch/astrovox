#include <math.h>
#include <ncurses.h>
#include <portaudio.h>
#include <portmidi.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "envelope.h"
#include "filter.h"
#include "input.h"
#include "main.h"
#include "oscillator.h"
#include "state.h"
#include "ui.h"

static int
audio_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
	float *out = (float *)outputBuffer;
	struct state *state = (struct state *)userData;
	(void)inputBuffer; // Prevent unused variable warnings
	(void)timeInfo;
	(void)statusFlags;

	float frequency = note_frequency(state->voices[0].note.value);
	float increment = frequency / SAMPLE_RATE;

	for (unsigned long i = 0; i < framesPerBuffer; i++) {
		float sample = 0.0f;

		sample = state->voices[0].osc.generator(state->time_index);
		state->time_index += increment;
		if (state->time_index >= 1.0f) {
			state->time_index -= 1.0f;
		}

		float env_value = env_process(&state->voices[0].env);
		sample *= env_value;
		sample = low_pass_filter_process(&state->voices[0].filter, sample);

		*out++ = sample; // Left channel
		*out++ = sample; // Right channel
	}

	return paContinue;
}

int
main(void)
{
	signal(SIGINT, sigint_handler);

	struct state state = {
		.voices[0] = init_voice(),
		.time_index = 0.0,
		.vis_time_index = 0.0
	};

	struct ui ui = init_ui();
	float last_frame_time = 0.0;

	// Initialize PortAudio
	PaError a_err= Pa_Initialize();
	if (a_err != paNoError) {
		fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(a_err));
		return 1;
	}

	// Open audio stream
	PaStream *stream;
	a_err= Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, paFramesPerBufferUnspecified, audio_callback, &state);
	if (a_err != paNoError) {
		fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(a_err));
		return 1;
	}

	a_err= Pa_StartStream(stream);
	if (a_err != paNoError) {
		fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(a_err));
		return 1;
	}

	while (1) {
		// Generate a waveform using the currently selected generator
		float vis_increment =
			state.voices[0].osc.frequency / SAMPLE_RATE * (BUFFER_SIZE / (float)(WINDOW_WIDTH - 2));
		// float waveform[BUFFER_SIZE];
		for (int i = 0; i < BUFFER_SIZE; i++) {
			state.vis_waveform[i] = state.voices[0].osc.generator(state.vis_time_index);
			state.vis_time_index += vis_increment;
			// loops correctly
			if (state.vis_time_index >= 1.0) {
				state.vis_time_index -= 1.0;
			}
		}

		// Find the maximum absolute value of the waveform
		float max_val = 0.0;
		for (int i = 0; i < BUFFER_SIZE; i++) {
			float abs_val = fabs(state.vis_waveform[i]);
			if (abs_val > max_val) {
				max_val = abs_val;
			}
		}

		// Display the waveform if it's time to redraw
		float current_time = (float)clock() / CLOCKS_PER_SEC;
		if (current_time - last_frame_time < ui.frame_duration) { continue; }
		last_frame_time = current_time;

		ui_tick(&ui, &state, BUFFER_SIZE, max_val);

		int c = getch();
		if (c == 'j') {
			prev_wave_gen(&state.voices[0].osc);
		} else if (c == 'k') {
			next_wave_gen(&state.voices[0].osc);
		} else if (c == 'h') { // Shift octave down
			if (state.voices[0].note.value > 11) {
				state.voices[0].note.value -= 12;
				state.voices[0].note.octave--;
			}
		} else if (c == 'l') { // Shift octave up
			if (state.voices[0].note.value < 75) {
				state.voices[0].note.value += 12;
				state.voices[0].note.octave++;
			}
		} else if (c == 'z') {
			// Decrease attack time
			state.voices[0].env.attack_time -= 0.05f;
			if (state.voices[0].env.attack_time < 0.0f) {
				state.voices[0].env.attack_time = 0.0f;
			}
		} else if (c == 'x') {
			// Increase attack time
			state.voices[0].env.attack_time += 0.05f;
			if (state.voices[0].env.attack_time > 1.0f) {
				state.voices[0].env.attack_time = 1.0f;
			}
		} else if (c == 'c') {
			// Decrease decay time
			state.voices[0].env.decay_time -= 0.05f;
			if (state.voices[0].env.decay_time < 0.0f) {
				state.voices[0].env.decay_time = 0.0f;
			}
		} else if (c == 'v') {
			// Increase decay time
			state.voices[0].env.decay_time += 0.05f;
			if (state.voices[0].env.decay_time > 1.0f) {
				state.voices[0].env.decay_time = 1.0f;
			}
		} else if (c == 'b') {
			// Decrease sustain level
			state.voices[0].env.sustain_level -= 0.05f;
			if (state.voices[0].env.sustain_level < 0.0f) {
				state.voices[0].env.sustain_level = 0.0f;
			}
		} else if (c == 'n') {
			// Increase sustain level
			state.voices[0].env.sustain_level += 0.05f;
			if (state.voices[0].env.sustain_level > 1.0f) {
				state.voices[0].env.sustain_level = 1.0f;
			}
		} else if (c == 'm') {
			// Decrease release time
			state.voices[0].env.release_time -= 0.05f;
			if (state.voices[0].env.release_time < 0.0f) {
				state.voices[0].env.release_time = 0.0f;
			}
		} else if (c == ',') {
			// Increase release time
			state.voices[0].env.release_time += 0.05f;
			if (state.voices[0].env.release_time > 1.0f) {
				state.voices[0].env.release_time = 1.0f;
			}
		} else if (c == 'a') {
			// Decrease cutoff frequency of low pass filter
			float cutoff = state.voices[0].filter.cutoff - 100.0f;
			low_pass_filter_set_cutoff(&state.voices[0].filter, cutoff);
		} else if (c == 's') {
			// Increase cutoff frequency of low pass filter
			float cutoff = state.voices[0].filter.cutoff + 100.0f;
			low_pass_filter_set_cutoff(&state.voices[0].filter, cutoff);
		} else if (c == 'd') {
			// Decrease resonance of low pass filter
			float resonance = state.voices[0].filter.resonance - 0.1f;
			low_pass_filter_set_resonance(&state.voices[0].filter, resonance);
		} else if (c == 'f') {
			// Increase resonance of low pass filter
			float resonance = state.voices[0].filter.resonance + 0.1f;
			low_pass_filter_set_resonance(&state.voices[0].filter, resonance);
		} else {
			const char *white_keys = "qwertyuiop";
			const char *black_keys = "23 567 90";
			const char *pos = strchr(white_keys, c);
			if (pos) {
				state.voices[0].note.value = (pos - white_keys) + 4 + (12 * state.voices[0].note.octave);
				state.voices[0].osc.frequency = note_frequency(state.voices[0].note.value);
				env_note_on(&state.voices[0].env);
			} else {
				pos = strchr(black_keys, c);
				if (pos) {
					state.voices[0].note.value = (pos - black_keys) + 5 + (12 * state.voices[0].note.octave);
					state.voices[0].osc.frequency = note_frequency(state.voices[0].note.value);
					env_note_on(&state.voices[0].env);
				} else {
					env_note_off(&state.voices[0].env);
				}
			}
		}
	}

	a_err = Pa_StopStream(stream);
	if (a_err != paNoError) {
		fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(a_err));
	}

	a_err = Pa_CloseStream(stream);
	if (a_err != paNoError) {
		fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(a_err));
	}

	Pa_Terminate();

	teardown_midi();

	endwin();
	return 0;
}

float
note_frequency(int note_pos)
{
	// NOTE: -1 means no note is being played and oscillator should be off.
	if (note_pos == -1)
		return 0.0;

	// NOTE: 49 is the position of A4 on a 88-key piano keyboard
	return A4_FREQUENCY * pow(2, (note_pos - 49) / 12.0);
}

void
sigint_handler(int sig)
{
	(void)sig; // Get rid of unused parameter warning
	endwin();
	printf("Exiting.\n");
	exit(0);
}
