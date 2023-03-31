#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "ui.h"
#include "waveforms.h"

int main(void) {
  signal(SIGINT, sigint_handler);

  wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen};
  int gen_count = sizeof generators / sizeof(wave_gen);
  struct juno_state state = { .gen_index = 0, .note = -1, .time_index = 0.0 };
  struct juno_ui ui = init_ui();
  double last_frame_time = 0.0;

  while (1) {
    // Generate a waveform using the currently selected generator
    double frequency = note_frequency(state.note);
    double increment =
        frequency / SAMPLE_RATE * (BUFFER_SIZE / (double)(WINDOW_WIDTH - 2));
    double waveform[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++) {
      waveform[i] = generators[state.gen_index](state.time_index);
      state.time_index += increment;
    }

    // Find the maximum absolute value of the waveform
    double max_val = 0.0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
      double abs_val = fabs(waveform[i]);
      if (abs_val > max_val) {
        max_val = abs_val;
      }
    }

    // Display the waveform if it's time to redraw
    double current_time = (double)clock() / CLOCKS_PER_SEC;
    if (current_time - last_frame_time < ui.frame_duration) { continue; }
    last_frame_time = current_time;

    ui_tick(&ui, &state, waveform, BUFFER_SIZE, max_val);

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

  endwin();
  return 0;
}

double note_frequency(int note_pos) {
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
