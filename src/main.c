#include <math.h>
#include <ncurses.h>
#include <portaudio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "ui.h"
#include "waveforms.h"

static int audio_callback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  float *out = (float *)outputBuffer;
  struct juno_state *state = (struct juno_state *)userData;
  (void)inputBuffer; // Prevent unused variable warnings
  (void)timeInfo;
  (void)statusFlags;

  for (unsigned long i = 0; i < framesPerBuffer; i++) {
    float sample = state->waveform[i % BUFFER_SIZE];
    *out++ = sample; // Left channel
    *out++ = sample; // Right channel
  }

  return paContinue;
}

int main(void) {
  signal(SIGINT, sigint_handler);

  wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen};
  int gen_count = sizeof generators / sizeof(wave_gen);
  struct juno_state state = { .gen_index = 0, .note = -1, .time_index = 0.0 };
  struct juno_ui ui = init_ui();
  float last_frame_time = 0.0;

  // Initialize PortAudio
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    return 1;
  }

  // Open audio stream
  PaStream *stream;
  err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE,
                             paFramesPerBufferUnspecified, audio_callback,
                             &state);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    return 1;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    return 1;
  }

  while (1) {
    // Generate a waveform using the currently selected generator
    float frequency = note_frequency(state.note);
    float increment =
        frequency / SAMPLE_RATE * (BUFFER_SIZE / (float)(WINDOW_WIDTH - 2));
    // float waveform[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++) {
      state.waveform[i] = generators[state.gen_index](state.time_index);
      state.time_index += increment;
      // Keep the time_index in the range [0.0, 1.0) to ensure the waveform
      // loops correctly
      if (state.time_index >= 1.0) {
        state.time_index -= 1.0;
      }
    }

    // Find the maximum absolute value of the waveform
    float max_val = 0.0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
      float abs_val = fabs(state.waveform[i]);
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
    if (c == 'q' || c == 'Q') {
      break;
    } else if (c == 'j') {
      state.gen_index = (state.gen_index - 1 + gen_count) % gen_count;
    } else if (c == 'k') {
      state.gen_index = (state.gen_index + 1) % gen_count;
    } else if (c == 'h') {
      state.note--;
    } else if (c == 'l') {
      state.note++;
    }
  }

  err = Pa_StopStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
  }

  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
  }

  Pa_Terminate();
  endwin();
  return 0;
}

float note_frequency(int note_pos) {
  // NOTE: -1 means no note is being played and oscillator should be off.
  if (note_pos == -1)
    return 0.0;

  // NOTE: 49 is the position of A4 on a 88-key piano keyboard
  return A4_FREQUENCY * pow(2, (note_pos - 49) / 12.0);
}

void sigint_handler(int sig) {
  (void)sig; // Get rid of unused parameter warning
  endwin();
  printf("Exiting.\n");
  exit(0);
}
